#include <boost/filesystem.hpp>
#include "ImageNaming.h"
#include "Formatter.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <ImageMagick-7/Magick++.h>

namespace fs = boost::filesystem;

void ImageNaming::convert()
{
    if (!fs::is_directory(src_dir_)) {
        Throw(src_dir_ + " not directory");
    }

    for (fs::directory_entry& x : fs::directory_iterator(src_dir_)) {

        if (fs::is_regular_file(x) && x.path().has_extension()) {

            std::string ext = x.path().extension().string();
            std::string filename = x.path().filename().string();

            if (!ext.empty() && ext.front() == '.') {
                ext.erase(0, 1);
            }

            boost::to_lower(ext);

            // std::cout << "    " << x.path() << " filename: " << x.path().filename()
            // << " extension: " << x.path().extension() << '\n';

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
//        item.parseAttributes();

        /*
        Magick::Image image;
        image.read(filename.data());

        // std::string attr_date_time = image.attribute("EXIF:DateTime");
        std::string attr_make = image.attribute("EXIF:Make");
        std::string attr_model = image.attribute("EXIF:Model");
         */

//        int count = 1;
        std::string new_name = formatter.rename(item);
//        std::string base = dest_dir_ + "/";
        //std::string capture_date, capture_time;

        /*
        {
            std::vector<std::string> v = parseDateTime(image);
            if (v.size() != 2) {
                Throw("Unhandled date formats '" + image.attribute("EXIF:DateTimeOriginal") + " " + image.attribute("EXIF:DateTime") + "'");
            }

            capture_date = v[0];
            capture_time = v[1];
        }
         */

//        base += boost::replace_all_copy(capture_date, ":", "");
//        base += "_";
//        // base += capture_time;
//        base += boost::replace_all_copy(capture_time, ":", "");



        /*
        do {
            new_name = base;
            if (count > 1) {
                new_name += "_";
                new_name += std::to_string(count);
            }

            new_name += "_";
            new_name += boost::replace_all_copy(attr_model, " ", "_");
            // new_name += fs::path(filename.data()).extension().native();
            new_name += ".";
            new_name += kOutputFormat;

        } while (fs::is_regular_file(new_name) && ++count);
         */

//        std::cout << "\tFile info '" << filename << "' [ "
//                  << " " << capture_date << "_" << capture_time << ", " << attr_make
//                  << ", " << attr_model << " ]";
//        std::cout << " --> " << new_name;
//        std::cout << "\n";



        if (enable_write_) {
//            image.write(new_name);
            item.write(new_name);
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error read filename " << filename << " : " << e.what() << "\n";
    }
}

void ImageNaming::init_libImageMagick(const char* path)
{
    Magick::InitializeMagick(path);
}
