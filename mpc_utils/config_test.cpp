#include "config.hpp"

#include <cstdlib>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "gtest/gtest.h"

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#include "windows.h"

std::wstring GetWC(const char* c) {
  const size_t cSize = strlen(c);
  std::wstring wc;
  wc.resize(cSize);
  mbstowcs((wchar_t*)wc.data(), c, cSize);
  return wc;
}

void setenv(const char* name, const wchar_t* val, int overwrite) {
  auto nl = strlen(name);
  std::wstring nn;

  SetEnvironmentVariable(GetWC(name).c_str(), val);
}

void unsetenv(const char* name) {
  SetEnvironmentVariable(GetWC(name).c_str(), NULL);
}

#endif

namespace mpc_utils {
namespace {

namespace po = boost::program_options;
namespace fs = boost::filesystem;

const int kNumConfigFiles = 3;

class DummyConfig : public virtual config {
 public:
  DummyConfig() : config(), value_(-1) {
    add_options()("value", po::value(&value_)->required(), "");
  }

  int value_;
};

class ConfigTest : public ::testing::Test {
 protected:
  ConfigTest() {
    for (int i = 0; i < kNumConfigFiles; i++) {
      fs::path path = fs::temp_directory_path() / fs::unique_path();
      path += ".ini";
      fs::ofstream file(path);
      file << "value = " << i << "\n";
      config_file_paths_.push_back(path);
    }
    setenv("MPC_UTILS_CONFIG", config_file_paths_[0].c_str(), 1);
  }

  ~ConfigTest() {
    for (const fs::path& path : config_file_paths_) {
      fs::remove(path);
    }
    unsetenv("MPC_UTILS_CONFIG");
  }

  DummyConfig config_;
  std::vector<fs::path> config_file_paths_;
};

TEST_F(ConfigTest, TestEnvConfigFile) {
  const char* args[] = {"test"};
  config_.parse(1, args);
  EXPECT_EQ(config_.value_, 0);
}

TEST_F(ConfigTest, TestCmdOverwritesConfigFile) {
  const char* args[] = {"test", "--value", "1"};
  config_.parse(3, args);
  EXPECT_EQ(config_.value_, 1);
}

TEST_F(ConfigTest, TestDefaultConfigFile) {
  unsetenv("MPC_UTILS_CONFIG");
  config_.set_default_filename(config_file_paths_[2].string());
  const char* args[] = {"test"};
  config_.parse(1, args);
  EXPECT_EQ(config_.value_, 2);
}

}  // namespace
}  // namespace mpc_utils