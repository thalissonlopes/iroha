/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_YAC_PB_CONVERTERS_HPP
#define IROHA_YAC_PB_CONVERTERS_HPP

#include "backend/protobuf/common_objects/proto_common_objects_factory.hpp"
#include "common/byteutils.hpp"
#include "consensus/yac/messages.hpp"
#include "cryptography/crypto_provider/crypto_defaults.hpp"
#include "interfaces/common_objects/signature.hpp"
#include "logger/logger.hpp"
#include "validators/field_validator.hpp"
#include "yac.pb.h"

namespace iroha {
  namespace consensus {
    namespace yac {
      class PbConverters {
       public:
        static proto::Vote serializeVotePayload(const VoteMessage &vote) {
          proto::Vote pb_vote;

          auto hash = pb_vote.mutable_hash();
          hash->set_block(vote.hash.block_hash);
          hash->set_proposal(vote.hash.proposal_hash);

          auto block_signature = hash->mutable_block_signature();

          block_signature->set_signature(shared_model::crypto::toBinaryString(
              vote.hash.block_signature->signedData()));

          block_signature->set_pubkey(shared_model::crypto::toBinaryString(
              vote.hash.block_signature->publicKey()));

          return pb_vote;
        }

        static proto::Vote serializeVote(const VoteMessage &vote) {
          proto::Vote pb_vote;

          auto hash = pb_vote.mutable_hash();
          hash->set_block(vote.hash.block_hash);
          hash->set_proposal(vote.hash.proposal_hash);

          auto block_signature = hash->mutable_block_signature();

          block_signature->set_signature(shared_model::crypto::toBinaryString(
              vote.hash.block_signature->signedData()));

          block_signature->set_pubkey(shared_model::crypto::toBinaryString(
              vote.hash.block_signature->publicKey()));

          auto signature = pb_vote.mutable_signature();
          const auto &sig = *vote.signature;
          signature->set_signature(
              shared_model::crypto::toBinaryString(sig.signedData()));
          signature->set_pubkey(
              shared_model::crypto::toBinaryString(sig.publicKey()));

          return pb_vote;
        }

        static boost::optional<VoteMessage> deserializeVote(
            const proto::Vote &pb_vote) {
          static shared_model::proto::ProtoCommonObjectsFactory<
              shared_model::validation::FieldValidator>
              factory_;

          VoteMessage vote;
          vote.hash.proposal_hash = pb_vote.hash().proposal();
          vote.hash.block_hash = pb_vote.hash().block();

          auto deserialize =
              [&](auto &pubkey, auto &signature, auto &val, const auto &msg) {
                factory_
                    .createSignature(shared_model::crypto::PublicKey(pubkey),
                                     shared_model::crypto::Signed(signature))
                    .match(
                        [&](iroha::expected::Value<
                            std::unique_ptr<shared_model::interface::Signature>>
                                &sig) { val = std::move(sig.value); },
                        [&](iroha::expected::Error<std::string> &reason) {
                          logger::log("YacPbConverter::deserializeVote")
                              ->error(msg, reason.error);
                        });
              };

          deserialize(pb_vote.hash().block_signature().pubkey(),
                      pb_vote.hash().block_signature().signature(),
                      vote.hash.block_signature,
                      "Cannot build vote hash block signature: {}");

          deserialize(pb_vote.signature().pubkey(),
                      pb_vote.signature().signature(),
                      vote.signature,
                      "Cannot build vote signature: {}");

          return vote;
        }
      };
    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha

#endif  // IROHA_YAC_PB_CONVERTERS_HPP