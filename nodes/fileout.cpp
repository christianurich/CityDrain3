#include "fileout.h"

#include <flow.h>

FileOut::FileOut()
	: Node()
{
	addParameter(ADD_PARAMETERS_P(out_file_name));
}

FileOut::~FileOut() {

}

void FileOut::f(int time, int dt) {
	(void) time;
	(void) dt;
}
