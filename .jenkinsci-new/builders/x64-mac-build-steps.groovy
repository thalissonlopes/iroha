#!/usr/bin/env groovy
/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

//
// Mac Build steps
//

def testSteps(scmVars, String buildDir, List environment, String testList) {
  withEnv(environment) {
      sh """
        export IROHA_POSTGRES_PASSWORD=${IROHA_POSTGRES_PASSWORD}; \
        export IROHA_POSTGRES_USER=${IROHA_POSTGRES_USER}; \
        mkdir -p /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}; \
        initdb -D /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}/ -U ${IROHA_POSTGRES_USER} --pwfile=<(echo ${IROHA_POSTGRES_PASSWORD}); \
        pg_ctl -D /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}/ -o '-p 5433 -c max_prepared_transactions=100' -l /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}/events.log start; \
        psql -h localhost -d postgres -p 5433 -U ${IROHA_POSTGRES_USER} --file=<(echo create database ${IROHA_POSTGRES_USER};)
      """
      sh "cd build; IROHA_POSTGRES_HOST=localhost IROHA_POSTGRES_PORT=5433 ctest --output-on-failure --no-compress-output --tests-regex '${testList}'  --test-action Test || true"

      sh 'python .jenkinsci-new/helpers/platform_tag.py "Darwin \$(uname -m)" \$(ls build/Testing/*/Test.xml)'
      // Mark build as UNSTABLE if there are any failed tests (threshold <100%)
      xunit testTimeMargin: '3000', thresholdMode: 2, thresholds: [passed(unstableThreshold: '100')], \
        tools: [CTest(deleteOutputFiles: true, failIfNotNew: false, \
        pattern: 'build/Testing/**/Test.xml', skipNoTestFiles: false, stopProcessingIfError: true)]

      sh """
        pg_ctl -D /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}/ stop && \
        rm -rf /var/jenkins/${scmVars.GIT_COMMIT}-${BUILD_NUMBER}/
      """
  }
}

def buildSteps(int parallelism, List compilerVersions, String build_type, boolean coverage, boolean testing, String testList, boolean packagebuild, List environment) {
  withEnv(environment) {
    scmVars = checkout scm
    def build = load '.jenkinsci-new/build.groovy'
    def vars = load ".jenkinsci-new/utils/vars.groovy"
    def utils = load ".jenkinsci-new/utils/utils.groovy"
    buildDir = 'build'
    compilers = vars.compilerMapping()
    cmakeBooleanOption = [ (true): 'ON', (false): 'OFF' ]
        cmakeBuildOptions = ""

    if (packagebuild){
      cmakeBuildOptions = " --target package "
    }

    utils.ccacheSetup(5)

    for (compiler in compilerVersions) {
      stage ("build ${compiler}"){
        build.cmakeConfigure(buildDir,
        "-DCMAKE_CXX_COMPILER=${compilers[compiler]['cxx_compiler']} \
        -DCMAKE_C_COMPILER=${compilers[compiler]['cc_compiler']} \
        -DCMAKE_BUILD_TYPE=${build_type} \
        -DCOVERAGE=${cmakeBooleanOption[coverage]} \
        -DTESTING=${cmakeBooleanOption[testing]} \
        -DPACKAGE_TGZ=${cmakeBooleanOption[packagebuild]} ")

        build.cmakeBuild(buildDir, cmakeBuildOptions, parallelism)
      }
      if (testing) {
        stage("Test ${compiler}") {
          coverage ? build.initialCoverage(buildDir) : echo('Skipping initial coverage...')
          testSteps(scmVars, buildDir, environment, testList)
          coverage ? build.postCoverage(buildDir, '/usr/local/bin/lcov_cobertura.py') : echo('Skipping post coverage...')
          // We run coverage once, using the first compiler as it is enough
          coverage = false
        }
      }
    }
  }
}

def successPostSteps(scmVars, boolean packagePush, List environment) {
  stage('Mac success PostSteps') {
    withEnv(environment) {
      timeout(time: 600, unit: "SECONDS") {
        if (packagePush) {
          def artifacts = load ".jenkinsci-new/artifacts.groovy"
          def commit = scmVars.GIT_COMMIT
          // if we use several compilers only the last compiler, used for the build, will be used for iroha.deb and iroha.tar.gz archives
          sh """
            ls -lah ./build
            mv ./build/iroha-*.tar.gz ./build/iroha.tar.gz
          """
          // publish packages
          filePaths = [ '\$(pwd)/build/*.tar.gz' ]
          artifacts.uploadArtifacts(filePaths, sprintf('iroha/macos/%1$s-%2$s-%3$s', [scmVars.GIT_LOCAL_BRANCH, sh(script: 'date "+%Y%m%d"', returnStdout: true).trim(), commit.substring(0,6)]))
        } else {
           archiveArtifacts artifacts: 'build/iroha*.tar.gz', allowEmptyArchive: true
        }
      }
    }
  }
}

def alwaysPostSteps(List environment) {
  stage('Mac always PostSteps') {
    withEnv(environment) {
      cleanWs()
    }
  }
}
return this