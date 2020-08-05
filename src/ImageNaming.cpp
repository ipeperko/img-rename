#include "ImageNaming.h"
#include "Formatter.h"
#include "Log.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

void ImageNaming::convert()
{
    if (!fs::is_directory(src_dir_)) {
        throw ::std::runtime_error(src_dir_ + " not directory");
    }

    for (fs::directory_entry& x : fs::directory_iterator(src_dir_)) {

        if (fs::is_regular_file(x) && x.path().has_extension()) {

            std::string ext = x.path().extension().string();
            std::string filename = x.path().filename().string();

            if (!ext.empty() && ext.front() == '.') {
                ext.erase(0, 1);
            }

            boost::to_lower(ext);

            Log(trace) << x.path() << " filename: " << x.path().filename() << " extension: " << x.path().extension();

            if (knownExtension(ext)) {
                processImage(x.path().native());
            }
        }
    }
}

bool ImageNaming::knownExtension(std::string_view ext)
{
    for (auto const& x : valid_extensions) {
        if (ext == x) {
            return true;
        }
    }

    return false;
}

void ImageNaming::processImage(std::string_view filename)
{
    try {
        FileItem item(filename);
        std::string new_name = formatter.rename(item);

        if (enable_write_) {
            item.write(new_name);
        }
    }
    catch (std::exception& e) {
        Log(error) << "Error read filename " << filename << " : " << e.what();
    }
}
