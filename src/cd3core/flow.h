#ifndef FLOW_H
#define FLOW_H

#include <boost/shared_array.hpp>
#include <vector>
#include <cd3globals.h>
#include <string>
#include <map>

struct FlowDefinition;

typedef std::vector<double> FlowPriv;


class CD3_PUBLIC Flow {
public:

	enum CalculationUnit {
		flow,
		concentration,
		rain,
		volume,
		null
	};

	Flow();
	Flow (const Flow &other);
	virtual ~Flow();

	Flow &operator =(const Flow &rhs);

	void setValue(const std::string &name,
				 double value);

	void clear();

	double getValue(const std::string &name) const;
	static CalculationUnit getUnit(const std::string &name);
	static size_t countUnits(CalculationUnit unit);

	double getIth(CalculationUnit unit, size_t i) const;
	void setIth(CalculationUnit unit, size_t i, double value);

	static const std::vector<std::string> &getNames();
	static const std::vector<std::string> &getUnitNames(CalculationUnit unit);
	static bool hasName(const std::string &name);
	static int size();

	void dump() const;

	static void define(std::map<std::string, CalculationUnit> definition);

	inline double &operator[](int i) {
		copyData();
		return f[i];
	}

	inline const double &operator[](int i) const {
		return f[i];
	}

private:
	static void addUnit(const std::string &name, CalculationUnit unit);
	static FlowDefinition fd;
	boost::shared_array<double> f;
	void copyData();
};

CD3_PUBLIC std::string cu2string(Flow::CalculationUnit c);
CD3_PUBLIC Flow::CalculationUnit string2cu(std::string s);


#endif // FLOW_H
