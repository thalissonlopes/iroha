#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

# Testing of transaction processor
addtest(transaction_processor_test transaction_processor_test.cpp)
target_link_libraries(transaction_processor_test
    processors
    shared_model_stateless_validation
    )

# Testing of query processor
addtest(query_processor_test query_processor_test.cpp)
target_link_libraries(query_processor_test
    processors
    shared_model_cryptography
    )
