#ifndef IVO_IMG_NAME_IMAGENAMING_H
#define IVO_IMG_NAME_IMAGENAMING_H

#include <stdexcept>
#include <string>
#include <vector>
#include "Formatter.h"

#define DEFAULT_DST_DIR "img_renamed"
#define Throw throw std::runtime_error

namespace Magick {
class Image;
}

class ImageNaming
{
public:
    // Init Imagemagick library (optional)
    static void init_libImageMagick(const char* path);

    ImageNaming() = default;

    void convert();

    void setSrcDirectory(std::string_view dir)
    {
        src_dir_ = dir;
    }

    std::string const& srcDirectory() const
    {
        return src_dir_;
    }

    void setDestDirectory(std::string_view dir)
    {
        formatter.setDestDirectory(dir);
    }

    std::string const& destDirectory() const
    {
        return formatter.destDirectory();
    }

    void setWriteEnabled(bool enable)
    {
        enable_write_ = enable;
    }

    bool writeEnabled() const
    {
        return enable_write_;
    }

    // Case insensitive
    static constexpr const char* valid_extensions[] = { "jpg", "jpeg", "heic" };

private:
    static bool knownExtension(std::string_view ext);
    void processImage(std::string_view filename);

    std::string src_dir_ { "./" };
    bool enable_write_ { true };
    Formatter formatter;

};

#endif //IVO_IMG_NAME_IMAGENAMING_H
