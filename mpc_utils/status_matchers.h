// Copyright 2019 Google LLC. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MPC_UTILS_STATUS_MATCHERS_H_
#define MPC_UTILS_STATUS_MATCHERS_H_

#include "gmock/gmock.h"
#include "absl/types/optional.h"
#include "mpc_utils/status.h"
#include "mpc_utils/status_macros.h"
#include "mpc_utils/statusor.h"

#define ASSERT_OK_AND_ASSIGN(lhs, rexpr) \
  ASSERT_OK_AND_ASSIGN_IMPL(             \
      STATUS_MACROS_CONCAT_NAME(_mpc_utils_statusor, __LINE__), lhs, rexpr)

#define ASSERT_OK_AND_ASSIGN_IMPL(statusor, lhs, rexpr) \
  auto statusor = (rexpr);                                   \
  ASSERT_THAT(statusor.status(), mpc_utils::IsOk());              \
  lhs = std::move(statusor).ValueOrDie();

namespace mpc_utils {
namespace internal {

class IsOkMatcher {
 public:
  template <typename StatusT>
  bool MatchAndExplain(const StatusT& status_container,
                       ::testing::MatchResultListener* listener) const {
    if (!status_container.ok()) {
      *listener << "which is not OK";
      return false;
    }
    return true;
  }

  void DescribeTo(std::ostream* os) const { *os << "is OK"; }

  void DescribeNegationTo(std::ostream* os) const { *os << "is not OK"; }
};

template <typename Enum>
class StatusIsMatcher {
 public:
  StatusIsMatcher(const StatusIsMatcher&) = default;
  StatusIsMatcher& operator=(const StatusIsMatcher&) = default;

  StatusIsMatcher(Enum code, absl::optional<absl::string_view> message)
      : code_{code}, message_{message} {}

  template <typename StatusT>
  bool MatchAndExplain(const StatusT& status,
                       ::testing::MatchResultListener* listener) const {
    if (code_ != status.code()) {
      *listener << "whose error code is generic::"
                << internal::CodeEnumToString(status.code());
      return false;
    }
    if (message_.has_value() && status.error_message() != message_.value()) {
      *listener << "whose error message is '" << message_.value() << "'";
      return false;
    }
    return true;
  }

  template <typename T>
  bool MatchAndExplain(const StatusOr<T>& status_or,
                       ::testing::MatchResultListener* listener) const {
    return MatchAndExplain(status_or.status(), listener);
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has a status code that is generic::"
        << internal::CodeEnumToString(code_);
    if (message_.has_value()) {
      *os << ", and has an error message that is '" << message_.value() << "'";
    }
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "has a status code that is not generic::"
        << internal::CodeEnumToString(code_);
    if (message_.has_value()) {
      *os << ", and has an error message that is not '" << message_.value()
          << "'";
    }
  }

 private:
  const Enum code_;
  const absl::optional<std::string> message_;
};

}  // namespace internal

inline ::testing::PolymorphicMatcher<internal::IsOkMatcher> IsOk() {
  return ::testing::MakePolymorphicMatcher(internal::IsOkMatcher{});
}

template <typename Enum>
::testing::PolymorphicMatcher<internal::StatusIsMatcher<Enum>> StatusIs(
    Enum code, absl::optional<absl::string_view> message = absl::nullopt) {
  return ::testing::MakePolymorphicMatcher(
      internal::StatusIsMatcher<Enum>{code, message});
}

}  // namespace mpc_utils

#endif // MPC_UTILS_STATUS_MATCHERS_H_