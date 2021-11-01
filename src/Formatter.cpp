#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "Formatter.h"
#include "Log.h"
#include <ImageMagick-7/Magick++.h>

namespace fs = boost::filesystem;

namespace Fn {
bool isFirst = [](FileItem& it) {
    return it.fileName().empty() || it.fileName().back() == '/';
};

Formatter::Appender captureDate = [](FileItem& it) {
    std::string s = it.captureDate();
    s = boost::replace_all_copy(s, ":", "");
    return s;
};

Formatter::Appender captureTime = [](FileItem& it) {
    std::string s = it.captureTime();
    s = boost::replace_all_copy(s, ":", "");
    return s;
};

Formatter::Appender cameraModel = [](FileItem& it) {
    std::string s = it.image().attribute("EXIF:Model");
    s = boost::replace_all_copy(s, " ", "_");
    return s;
};

Formatter::Appender cameraMake = [](FileItem& it) {
    std::string s = it.image().attribute("EXIF:Make");
    s = boost::replace_all_copy(s, " ", "_");
    return s;
};

Formatter::Appender delimiter = [](FileItem& it) {
    return "_";
};
} // namespace Fn

Formatter::Formatter()
{
    fs.emplace_back(Fn::captureDate);
    fs.emplace_back(Fn::delimiter);
    fs.emplace_back(Fn::captureTime);
    fs.emplace_back(Fn::delimiter);
    fs.emplace_back(Fn::cameraModel);
}

std::string Formatter::rename(FileItem& file)
{
    file.temp.count_fn = 1;
    std::string& new_name = file.temp.new_filename;

    do {
        if (!dest_dir.empty()) {
            new_name = dest_dir + "/";
        }
        else {
            new_name = "";
        }

        for (const auto& f : fs) {
            new_name += f(file);
        }

        if (file.temp.count_fn > 1) {
            new_name += "_" + std::to_string(file.temp.count_fn);
        }

        new_name += ".";
        new_name += kOutputFormat;
    } while (fs::is_regular_file(new_name) && ++file.temp.count_fn);

    Log(info) << "Rename file '" << file.fileName() <<
        "' [" << (!file.captureDate().empty() ? file.captureDate() : std::string("unknown date")) <<
        "_" << (!file.captureTime().empty() ? file.captureTime() : std::string("unknown time")) << " "
              << (!file.image().attribute("EXIF:Make").empty() ? file.image().attribute("EXIF:Make") : std::string("unknown manufacturer")) << ", "
              << (!file.image().attribute("EXIF:Model").empty() ? file.image().attribute("EXIF:Model") : std::string("unknown model")) << "]"
              << " --> " << new_name;

    return new_name;
}

void Formatter::setDestDirectory(std::string_view dir)
{
    std::string tmp { dir };
    while (!tmp.empty() && tmp.back() == '/') {
        tmp.pop_back();
    }
    dest_dir = std::move(tmp);
}

void Formatter::setFormat(std::string_view format)
{
    auto p1 = format.begin();
    auto p2 = p1;

    auto get_expr = [&]() {
        size_t n = p2 - p1;
        if (n) {
            std::string s { p1, 0, n };
            fs.push_back([s](FileItem&) {
                return s;
            });
        }
    };

    fs.clear();

    while (p2 != format.end()) {

        if (*p2 == '%') {
            get_expr();

            p1 = p2;
            p2 = p1 + 1;

            if (p2 >= format.end()) {
                throw std::overflow_error("Parse error");
            }

            if (*p2 == 'D') {
                fs.push_back(Fn::captureDate);
                p1 = p2 = p2 + 1;
            }
            else if (*p2 == 'T') {
                fs.push_back(Fn::captureTime);
                p1 = p2 = p2 + 1;
            }
            else if (*p2 == 'M') {
                fs.push_back(Fn::cameraMake);
                p1 = p2 = p2 + 1;
            }
            else if (*p2 == 'm') {
                fs.push_back(Fn::cameraModel);
                p1 = p2 = p2 + 1;
            }
            else {
                throw std::domain_error("Invalid format");
            }
        }
        else {
            ++p2;
        }
    }

    get_expr();
}
