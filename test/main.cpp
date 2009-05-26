#include <saxloader.h>
#include <simulation.h>
#include <cd3assert.h>
#include <mapbasedmodel.h>

#include <iostream>
#include <QFile>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#ifdef __MINGW32__
/* Required header file */
#include <fcntl.h>
#endif


void progress(int percent) {
	std::cout << "                                       \r";
	std::cout << "progress: " << percent << "%\n";
	std::cout.flush();
}


struct PerStateHandler {
	PerStateHandler(const std::string dir) {
		state_dir = dir;
	}
	void operator()(ISimulation *s, int time) {
		(void) time;
		s->serialize(state_dir);
	}
private:
	std::string state_dir;
};

/*int main(int argc, char **argv) {
#ifdef __MINGW32__
	// Switch to binary mode
	_setmode(_fileno(stdout),_O_BINARY);
	_setmode(_fileno(stdin),_O_BINARY);
	_setmode(_fileno(stderr),_O_BINARY);
#endif
	if (argc < 2) {
		std::cout << "provide model file" << std::endl;
		return 1;
	}
	MapBasedModel m;
	SaxLoader loader(&m);
	QFile f(argv[1]);

	ISimulation *s = loader.load(f);
	s->setModel(&m);

	int starttime = s->getSimulationParameters().start;

	s->start(starttime);
	f.close();
	delete s;
	return 0;
}*/

int main(int argc, char **argv) {
	po::options_description desc("CityDrain3 command line options");

	desc.add_options()
		("help,h", "produce help message")
		("state-dir,d", po::value<std::string>(), "used to store and locate restart data")
		("restart,r", po::value<int>(), "specifiy the time where to restart")
		("model,m", po::value<std::string>(), "the model to run the simulation");
	;

	po::positional_options_description p;
	p.add("model", 1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 1;
	}

	if (!vm.count("model")) {
		std::cerr << "provide a model file" << std::endl;
		std::cout << desc << std::endl;
		//std::cout << p << std::endl;
		return -1;
	}

	QFile f(QString::fromStdString(vm["model"].as<std::string>()));

	MapBasedModel m;

	SaxLoader loader(&m);

	ISimulation *s = loader.load(f);
	cd3assert(s, "simulation loading failed");

	if (vm.count("state-dir")) {
		s->timestep.connect(PerStateHandler(vm["state-dir"].as<std::string>()));
	}

	s->progress.connect(progress);
	s->setModel(&m);

	int starttime = s->getSimulationParameters().start;

	if (vm.count("restart")) {
		if (!vm.count("state-dir")) {
			std::cerr << "provide state-dir with restart option" << std::endl;
			std::cerr << desc << std::endl;
			return -1;
		}
		starttime = vm["restart"].as<int>();
		std::cout << "restarting from " << starttime <<  std::endl;
		s->deserialize(vm["state-dir"].as<std::string>(), starttime);
	}

	s->start(starttime);

	std::cout << std::endl; //newline the progress
	//s->serialize("states");
	delete s;
	return 0;
}
