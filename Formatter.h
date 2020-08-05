#ifndef IMG_RENAME_FORMATTER_H
#define IMG_RENAME_FORMATTER_H

#include <string>
#include <vector>
#include <functional>
#include "FileItem.h"

namespace Magick {
class Image;
}

class Formatter
{
public:
    using Appender = std::function<std::string(FileItem& it)>;

    Formatter();

    std::string rename(FileItem& file);

    void setDestDirectory(std::string_view dir);

    std::string const& destDirectory() const
    {
        return dest_dir;
    }

    void setFormat(std::string_view format);

private:
    std::vector<Appender> fs;
    std::string dest_dir;
    static constexpr const char* kOutputFormat = "jpg";
};

#endif //IMG_RENAME_FORMATTER_H
