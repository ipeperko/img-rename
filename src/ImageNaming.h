#ifndef IMG_RENAME_IMAGENAMING_H
#define IMG_RENAME_IMAGENAMING_H

#include "Formatter.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_set>

#define DEFAULT_DST_DIR "img_renamed"
#define DEFAULT_UNHANDLED_SUBDIR "unhandled"

namespace Magick {
class Image;
}

class ImageNaming
{
public:
    ImageNaming() = default;

    void runDirectory();

    void setSrcDirectory(std::string_view dir)
    {
        src_dir_ = dir;
    }

    auto const& srcDirectory() const
    {
        return src_dir_;
    }

    void setDestDirectory(std::string_view dir)
    {
        formatter.setDestDirectory(dir);
    }

    auto const& destDirectory() const
    {
        return formatter.destDirectory();
    }

    std::string unhandledDirectory() const;

    void setWriteEnabled(bool enable)
    {
        enable_write_ = enable;
    }

    bool writeEnabled() const
    {
        return enable_write_;
    }

    void setCopyUnhandled(bool val)
    {
        copy_unhandled_ = val;
    }

    bool copyUnhandled() const
    {
        return copy_unhandled_;
    }

    void setFormat(std::string_view format)
    {
        formatter.setFormat(format);
    }

    void readSettingsFile();

    // Case insensitive
    static constexpr std::array<std::string_view, 3> valid_extensions { "jpg", "jpeg", "heic" };

private:
    static bool knownExtension(std::string_view ext);
    void processImage(std::string_view filename);

    std::string src_dir_ { "./" };
    bool enable_write_ { true };
    bool copy_unhandled_ {false};
    std::unordered_set<std::string> unhandled_;
    Formatter formatter;
};

#endif //IMG_RENAME_IMAGENAMING_H
