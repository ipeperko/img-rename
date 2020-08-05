#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>
#include "ImageNaming.h"
#include "Log.h"
#include <ImageMagick-7/Magick++.h>

#define VERSION "0.0.2"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

void printHeader()
{
    std::ostringstream s;
    s << "======================================\n";
    s << " Images rename tool\n";
    s << " version " VERSION "\n";
    s << " [ ";
    for (auto const& x : ImageNaming::valid_extensions) {
        s << x << " ";
    }
    s << "]\n";
    s << "======================================\n";
    s << "\n";
    std::cout << s.str();
    std::cout.flush();
}

int main(int argc, char** argv) try
{
    printHeader();

    Magick::InitializeMagick(*argv);
    ImageNaming nmg;

    po::options_description generic_options("Options");
    generic_options.add_options()
        ("help,h", "help message")
        ("version,v", "display version")
        ("src-dir,s", po::value<std::string>(), "source directory")
        ("dest-dir,d", po::value<std::string>(), "destination directory directory\n"
                                                 "default: {src-dir}/" DEFAULT_DST_DIR)
        ("format,f", po::value<std::string>(), "image name format (e.g. %D_%T_%M_%m)")
        ("disable-write", "disable write output files")
        ("log-level", po::value<std::string>(), "set logging level [error|warning|info|debug|trace]");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, generic_options), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << generic_options << "\n";
        exit(EXIT_SUCCESS);
    }
    if (vm.count("version")) {
        std::cout << "Version: " << VERSION << "\n";
        exit(EXIT_SUCCESS);
    }

    // Log level
    if (vm.count("log-level")) {
        std::string level = vm["log-level"].as<std::string>();
        try {
            Log::setLoggingLevel(level);
        }
        catch (std::exception& e) {
            throw;
        }
    }

    // Read settings file
    nmg.readSettingsFile();

    // Disable witing option
    if (vm.count("disable-write")) {
        nmg.setWriteEnabled(false);
    }

    // Image name format
    if (vm.count("format")) {
        nmg.setFormat(vm["format"].as<std::string>());
    }

    // Source directory init
    if (vm.count("src-dir")) {
        nmg.setSrcDirectory(vm["src-dir"].as<std::string>());
    }
    else {
        throw std::domain_error("source folder not specified");
    }

    // Destination directory init
    if (nmg.writeEnabled()) {

        // Destination directory name
        if (vm.count("dest-dir")) {
            nmg.setDestDirectory(vm["dest-dir"].as<std::string>());
        }
        else {
            std::string dest_dir(nmg.srcDirectory());
            if (dest_dir.empty()) {
                dest_dir += "./";
            }
            else if (!nmg.srcDirectory().empty() && nmg.srcDirectory().back() != '/') {
                dest_dir += '/';
            }
            dest_dir += DEFAULT_DST_DIR;
            nmg.setDestDirectory(dest_dir);
        }

        // Create destination directory
        fs::path p(nmg.destDirectory());

        if (!fs::is_directory(p)) {
            std::cout << "Creating directory " << p.native() << "\n";
            fs::create_directory(p);
        }
    }

    // Convert files
    nmg.convert();

    exit(EXIT_SUCCESS);
}
catch (std::exception& e)
{
    Log(error) << e.what();
    exit(EXIT_FAILURE);
}
