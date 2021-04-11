#pragma once

#include <filesystem>

using Path = std::filesystem::path;
inline Path get_out_dir_path(){
    static const Path self_path(__FILE__);
    return self_path.parent_path() / "out";
}
