#include "Server.h"
#include <vector>
#include <boost/program_options.hpp>

using namespace std;

int main(int argc, char** argv)
{
	namespace po = boost::program_options;

	stringstream usage;
	usage << "Usage: " << argv[0] << " -h | -p ";
	po::options_description desc(usage.str().c_str());
	desc.add_options()
	  ("help,h", "Print help messages")
	  ("xml,x", po::value<std::string>(), "Xml for respondong to requests")
	  ("file,f", po::value<std::string>(), "Path to xml with policy")
	  ("ports,p", po::value<vector<uint16_t>>(), "Ports to listen requests, by default 843 port used")
	  ("daemon,d", "Use daemon function to detach pipes and daemonize server");

	po::variables_map vm;

	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
	}
	catch(po::error &e)
	{
		std::cout << desc;
		abort();
	}

	if(vm.count("help"))
	{
		std::cout << desc;
		return 0;
	}

	Server::ports_t ports = {{ 843 }};

	Server server;
	server.set_ports(move(ports));
	server.run();

	return 0;
}

