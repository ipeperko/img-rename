#include "ImageNaming.h"
#include "Formatter.h"
#include "Log.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>
#include <pwd.h>
#include <iostream>

namespace fs = boost::filesystem;

void ImageNaming::runDirectory()
{
    unhandled_.clear();

    if (!fs::is_directory(src_dir_)) {
        throw ::std::runtime_error(src_dir_ + " not directory");
    }

    for (fs::directory_entry& x : fs::directory_iterator(src_dir_)) {

        if (fs::is_regular_file(x) && x.path().has_extension()) {

            std::string ext = x.path().extension().string();
            std::string filename = x.path().filename().string();

            if (!ext.empty() && ext.front() == '.')
                ext.erase(0, 1);

            boost::to_lower(ext);

            Log(trace) << x.path() << " filename: " << x.path().filename() << " extension: " << x.path().extension();

            if (knownExtension(ext))
                processImage(x.path().native());
        }
    }

    if (copy_unhandled_) {

        for (auto const& file : unhandled_) {

            try {
                fs::path path(file);
                std::string new_name = unhandledDirectory() + "/" + path.filename().string();

                Log(info) << "Copy unhandled file " << file << " -> " << new_name;

                if (enable_write_)
                    fs::copy_file(file, new_name, fs::copy_option::overwrite_if_exists);
            }
            catch (std::exception& e) {
                Log(error) << "Copy unhandled file failed - " << e.what();
            }
        }
    }
}

std::string ImageNaming::unhandledDirectory() const
{
    return (formatter.destDirectory().empty() ? std::string() : formatter.destDirectory() + "/") + DEFAULT_UNHANDLED_SUBDIR;
}

bool ImageNaming::knownExtension(std::string_view ext)
{
    return std::any_of(valid_extensions.begin(), valid_extensions.end(), [&](std::string_view e) { return ext == e; });
}

void ImageNaming::processImage(std::string_view filename)
{
    try {
        FileItem item(filename.data());
        std::string new_name = formatter.rename(item);

        if (enable_write_)
            item.write(new_name);
    }
    catch (std::exception& e) {
        if (copy_unhandled_)
            unhandled_.insert(filename.data());

        Log(error) << "Error read filename " << filename << " : " << e.what();
    }
}

void ImageNaming::readSettingsFile()
{
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string fname;

    if (homedir) {
        fname += homedir;
    }
    if (!fname.empty()) {
        fname += "/";
    }
    fname += ".img_rename.conf";

    std::ifstream fi(fname);
    if (!fi.is_open()) {
        Log(trace) << "No config file found or cannot open " << fname;
        return;
    }

    Log(trace) << "Reading config file " << fname;
    std::string line;

    while (std::getline(fi, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t p;
        if ((p = line.find("format=")) != std::string::npos) {
            std::string format = line.substr(p + strlen("format="));
            Log(trace) << "Config file format : " << format;
            setFormat(format);
        }
    }
}
