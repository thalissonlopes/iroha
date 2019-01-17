/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "module/shared_model/mock_objects_factories/mock_command_factory.hpp"
#include "backend/protobuf/permissions.hpp"
#include "utils/string_builder.hpp"

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnRefOfCopy;

namespace shared_model {
  namespace interface {
    template <typename CommandType, typename ExpectsCallable>
    MockCommandFactory::FactoryResult<CommandType>
    MockCommandFactory::createFactoryResult(
        ExpectsCallable expects_callable) const {
      auto specific_cmd_mock = std::make_unique<CommandType>();
      return expects_callable(std::move(specific_cmd_mock));
    }

    MockCommandFactory::FactoryResult<MockAddAssetQuantity>
    MockCommandFactory::constructAddAssetQuantity(
        const types::AssetIdType &asset_id, const Amount &asset_amount) const {
      return createFactoryResult<MockAddAssetQuantity>(
          [&asset_id, &asset_amount](
              FactoryResult<MockAddAssetQuantity> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, assetId())
                .WillByDefault(ReturnRefOfCopy(asset_id));
            ON_CALL(*specific_cmd_mock, amount())
                .WillByDefault(ReturnRefOfCopy(asset_amount));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockAddPeer>
    MockCommandFactory::constructAddPeer(const Peer &peer) const {
      return createFactoryResult<MockAddPeer>(
          [&peer](FactoryResult<MockAddPeer> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, peer()).WillByDefault(ReturnRef(peer));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockAddSignatory>
    MockCommandFactory::constructAddSignatory(
        const types::PubkeyType &pubkey,
        const types::AccountIdType &account_id) const {
      return createFactoryResult<MockAddSignatory>(
          [&pubkey,
           &account_id](FactoryResult<MockAddSignatory> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, pubkey())
                .WillByDefault(ReturnRefOfCopy(pubkey));
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockAppendRole>
    MockCommandFactory::constructAppendRole(
        const types::AccountIdType &account_id,
        const types::RoleIdType &role_name) const {
      return createFactoryResult<MockAppendRole>(
          [&account_id,
           &role_name](FactoryResult<MockAppendRole> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, roleName())
                .WillByDefault(ReturnRefOfCopy(role_name));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockCreateAccount>
    MockCommandFactory::constructCreateAccount(
        const types::AccountNameType &account_name,
        const types::DomainIdType &domain_id,
        const types::PubkeyType &pubkey) const {
      return createFactoryResult<MockCreateAccount>(
          [&account_name, &domain_id, &pubkey](
              FactoryResult<MockCreateAccount> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountName())
                .WillByDefault(ReturnRefOfCopy(account_name));
            ON_CALL(*specific_cmd_mock, domainId())
                .WillByDefault(ReturnRefOfCopy(domain_id));
            ON_CALL(*specific_cmd_mock, pubkey())
                .WillByDefault(ReturnRefOfCopy(pubkey));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockCreateAsset>
    MockCommandFactory::constructCreateAsset(
        const types::AssetNameType &asset_name,
        const types::DomainIdType &domain_id,
        const types::PrecisionType &precision) const {
      return createFactoryResult<MockCreateAsset>(
          [&asset_name, &domain_id, &precision](
              FactoryResult<MockCreateAsset> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, assetName())
                .WillByDefault(ReturnRefOfCopy(asset_name));
            ON_CALL(*specific_cmd_mock, domainId())
                .WillByDefault(ReturnRefOfCopy(domain_id));
            ON_CALL(*specific_cmd_mock, precision())
                .WillByDefault(ReturnRefOfCopy(precision));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockCreateDomain>
    MockCommandFactory::constructCreateDomain(
        const types::DomainIdType &domain_id,
        const types::RoleIdType &role_id) const {
      return createFactoryResult<MockCreateDomain>(
          [&domain_id,
           &role_id](FactoryResult<MockCreateDomain> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, domainId())
                .WillByDefault(ReturnRefOfCopy(domain_id));
            ON_CALL(*specific_cmd_mock, userDefaultRole())
                .WillByDefault(ReturnRefOfCopy(role_id));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockCreateRole>
    MockCommandFactory::constructCreateRole(
        const types::RoleIdType &role_id,
        const RolePermissionSet &role_permissions) const {
      return createFactoryResult<MockCreateRole>(
          [&role_id,
           &role_permissions](FactoryResult<MockCreateRole> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, roleName())
                .WillByDefault(ReturnRefOfCopy(role_id));
            ON_CALL(*specific_cmd_mock, rolePermissions())
                .WillByDefault(ReturnRefOfCopy(role_permissions));
            ON_CALL(*specific_cmd_mock, toString())
                .WillByDefault(Return(
                    detail::PrettyStringBuilder()
                        .init("CreateRole")
                        .append("role_name", role_id)
                        .appendAll(shared_model::proto::permissions::toString(
                                       role_permissions),
                                   [](auto p) { return p; })
                        .finalize()));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockDetachRole>
    MockCommandFactory::constructDetachRole(
        const types::AccountIdType &account_id,
        const types::RoleIdType &role_id) const {
      return createFactoryResult<MockDetachRole>(
          [&account_id,
           &role_id](FactoryResult<MockDetachRole> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, roleName())
                .WillByDefault(ReturnRefOfCopy(role_id));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockGrantPermission>
    MockCommandFactory::constructGrantPermission(
        const types::AccountIdType &account_id,
        permissions::Grantable permission) const {
      return createFactoryResult<MockGrantPermission>(
          [&account_id,
           permission](FactoryResult<MockGrantPermission> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, permissionName())
                .WillByDefault(Return(permission));
            ON_CALL(*specific_cmd_mock, toString())
                .WillByDefault(Return(
                    detail::PrettyStringBuilder()
                        .init("GrantPermission")
                        .append("account_id", account_id)
                        .append("permission",
                                shared_model::proto::permissions::toString(
                                    permission))
                        .finalize()));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockRemoveSignatory>
    MockCommandFactory::constructRemoveSignatory(
        const types::AccountIdType &account_id,
        const types::PubkeyType &pubkey) const {
      return createFactoryResult<MockRemoveSignatory>(
          [&account_id,
           &pubkey](FactoryResult<MockRemoveSignatory> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, pubkey())
                .WillByDefault(ReturnRefOfCopy(pubkey));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockRevokePermission>
    MockCommandFactory::constructRevokePermission(
        const types::AccountIdType &account_id,
        permissions::Grantable permission) const {
      return createFactoryResult<MockRevokePermission>(
          [&account_id,
           permission](FactoryResult<MockRevokePermission> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, permissionName())
                .WillByDefault(Return(permission));
            ON_CALL(*specific_cmd_mock, toString())
                .WillByDefault(Return(
                    detail::PrettyStringBuilder()
                        .init("RevokePermission")
                        .append("account_id", account_id)
                        .append("permission",
                                shared_model::proto::permissions::toString(
                                    permission))
                        .finalize()));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockSetAccountDetail>
    MockCommandFactory::constructSetAccountDetail(
        const types::AccountIdType &account_id,
        const types::AccountDetailKeyType &cmd_key,
        const types::AccountDetailValueType &cmd_value) const {
      return createFactoryResult<MockSetAccountDetail>(
          [&account_id, &cmd_key, &cmd_value](
              FactoryResult<MockSetAccountDetail> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, key())
                .WillByDefault(ReturnRefOfCopy(cmd_key));
            ON_CALL(*specific_cmd_mock, value())
                .WillByDefault(ReturnRefOfCopy(cmd_value));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockSetQuorum>
    MockCommandFactory::constructSetQuorum(
        const types::AccountIdType &account_id,
        types::QuorumType quorum) const {
      return createFactoryResult<MockSetQuorum>(
          [&account_id,
           quorum](FactoryResult<MockSetQuorum> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, accountId())
                .WillByDefault(ReturnRefOfCopy(account_id));
            ON_CALL(*specific_cmd_mock, newQuorum())
                .WillByDefault(Return(quorum));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockSubtractAssetQuantity>
    MockCommandFactory::constructSubtractAssetQuantity(
        const types::AssetIdType &asset_id, const Amount &cmd_amount) const {
      return createFactoryResult<MockSubtractAssetQuantity>(
          [&asset_id, &cmd_amount](
              FactoryResult<MockSubtractAssetQuantity> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, assetId())
                .WillByDefault(ReturnRefOfCopy(asset_id));
            ON_CALL(*specific_cmd_mock, amount())
                .WillByDefault(ReturnRefOfCopy(cmd_amount));
            return specific_cmd_mock;
          });
    }

    MockCommandFactory::FactoryResult<MockTransferAsset>
    MockCommandFactory::constructTransferAsset(
        const types::AccountIdType &src_account_id,
        const types::AccountIdType &dest_account_id,
        const types::AssetIdType &asset_id,
        const types::DescriptionType &cmd_description,
        const Amount &cmd_amount) const {
      return createFactoryResult<MockTransferAsset>(
          [&src_account_id,
           &dest_account_id,
           &asset_id,
           &cmd_description,
           &cmd_amount](FactoryResult<MockTransferAsset> specific_cmd_mock) {
            ON_CALL(*specific_cmd_mock, srcAccountId())
                .WillByDefault(ReturnRefOfCopy(src_account_id));
            ON_CALL(*specific_cmd_mock, destAccountId())
                .WillByDefault(ReturnRefOfCopy(dest_account_id));
            ON_CALL(*specific_cmd_mock, assetId())
                .WillByDefault(ReturnRefOfCopy(asset_id));
            ON_CALL(*specific_cmd_mock, description())
                .WillByDefault(ReturnRefOfCopy(cmd_description));
            ON_CALL(*specific_cmd_mock, amount())
                .WillByDefault(ReturnRefOfCopy(cmd_amount));
            return specific_cmd_mock;
          });
    }
  }  // namespace interface
}  // namespace shared_model
