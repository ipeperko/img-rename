#ifndef IMG_RENAME_FILEITEM_H
#define IMG_RENAME_FILEITEM_H

#include <string>
#include <unordered_map>
#include <optional>
#include <memory>
#include <vector>

namespace Magick {
class Image;
}

class FileItem
{
public:
    FileItem();
    explicit FileItem(std::string_view file_name);

    std::string const& fileName() const { return filename; }

    Magick::Image& image();

    Magick::Image const& image() const;

    void write(std::string file_name);

    void parseAttributes();

    static std::vector<std::string> parseDateTime(Magick::Image& image);

    std::string const captureDate() const { return capture_date; }

    std::string const captureTime() const { return capture_time; }

    std::string tmp_new_filename; // temporary file name (used by formatter)
    int tmp_count_fn {0}; // count files with same file name (used by formatter)

private:
    std::shared_ptr<Magick::Image> img;
    std::string filename;
//    std::unordered_map<std::string, std::string> attrs;

//    std::string attr_make;
//    std::string attr_model;
    std::string capture_date;
    std::string capture_time;
};

#endif //IMG_RENAME_FILEITEM_H
