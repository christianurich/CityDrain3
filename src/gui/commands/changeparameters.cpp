#include "changeparameters.h"
#include <simulationscene.h>
#include <mapbasedmodel.h>
#include <node.h>

ChangeParameters::ChangeParameters(SimulationScene *scene,
								   NodeItem *node,
								   SavedParameters before,
								   std::string id_before)
	: scene(scene),
	before(before), after(node->saveParameters()),
	id_before(id_before), id_after(node->getId().toStdString()) {

}

ChangeParameters::~ChangeParameters() {
}

void ChangeParameters::undo() {
	NodeItem *item = scene->findItem(QString::fromStdString(id_after));
	Q_ASSERT(item != 0);
	scene->model->renameNode(item->getNode(), id_before);
	item->restoreParameters(before);
	SimulationParameters sp = scene->simulation->getSimulationParameters();
	item->getNode()->init(sp.start, sp.stop, sp.dt);
	item->updatePorts();
	scene->update();
}

void ChangeParameters::redo() {
	static bool first_redo = true;
	if (first_redo) {
		first_redo = false;
		return;
	}
	NodeItem *item = scene->findItem(QString::fromStdString(id_before));
	Q_ASSERT(item != 0);
	scene->model->renameNode(item->getNode(), id_after);
	item->restoreParameters(after);
	SimulationParameters sp = scene->simulation->getSimulationParameters();
	item->getNode()->init(sp.start, sp.stop, sp.dt);
	item->updatePorts();
	scene->update();
}
