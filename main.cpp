#include "Server.h"
#include <vector>
#include <boost/program_options.hpp>
#include "easylogging++.h"
#include <fstream>
#include <iosfwd>
#include <sstream>
#include <string>

INITIALIZE_EASYLOGGINGPP

using namespace std;

int main(int argc, char** argv)
{
	namespace po = boost::program_options;

	typedef vector<uint16_t> ports_params_t;

	stringstream usage;
	usage << "Usage: " << argv[0] << " [ -h | -d [-p <port>]... [-x <your_xml> | -f <path_to_xml>] ] \n" ;

	po::options_description desc(usage.str().c_str());
	desc.add_options()
	  ("help,h", "Print help messages")
	  ("xml,x", po::value<std::string>(), "Xml for respondong to requests")
	  ("file,f", po::value<std::string>(), "Path to xml with policy")
	  ("ports,p", po::value<ports_params_t>(), "Ports to listen requests, by default 843 port used")
	  ("daemon,d", "Use daemon function to detach pipes and daemonize server");

	po::variables_map vm;

	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
	}
	catch (po::error &e)
	{
		std::cout << desc;
		abort();
	}

	if (vm.count("help"))
	{
		std::cout << desc;
		return 0;
	}

	ports_params_t ports_params = {{ 843 }};
	string text;

	if (vm.count("xml"))
	{
		text = vm["xml"].as<string>();
		LOG(INFO) << "Load policy xml from params \"" << text << "\"";
	}
	else if (vm.count("file"))
	{
		string file_path = vm["file"].as<string>();
		ifstream file;
		file.open(file_path);

		if(!file.is_open())
			LOG(FATAL) << desc << "\n" << "Can't open file " << file_path;

		stringstream text_stream;
		string buffer;

		while (getline (file, buffer))
			text_stream << buffer;

		text = text_stream.str();

		LOG(INFO) << "Load policy xml from file " << file_path << " : "
			<< "\"" << text << "\"";

		file.close();
	}

	if(text.empty())
		LOG(ERROR) << "Your xml is empty";

	if (vm.count("ports"))
		ports_params = vm["ports"].as<ports_params_t>();

	Server::ports_t ports;

	for(auto &port: ports_params)
		ports.insert(port);

	Server server;
	server.set_ports(move(ports));
	server.set_text(text);
	server.run();

	return 0;
}

