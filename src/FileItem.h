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
    struct Temp
    {
        std::string new_filename; // temporary file name (used by formatter)
        int count_fn {0}; // count files with same file name (used by formatter)
    } temp;

    FileItem();
    explicit FileItem(std::string file_name);

    auto const& fileName() const { return filename; }

    Magick::Image& image();

    Magick::Image const& image() const;

    void write(std::string file_name);

    void parseAttributes();

    static std::vector<std::string> parseDateTime(Magick::Image& image);

    auto const& captureDate() const { return capture_date; }

    auto const& captureTime() const { return capture_time; }

private:
    std::shared_ptr<Magick::Image> img;
    std::string filename;
    std::string capture_date;
    std::string capture_time;
};

#endif //IMG_RENAME_FILEITEM_H
