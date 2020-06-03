#include "variablesdefinition.h"
#include <cmath>
VariablesDefinition::VariablesDefinition()
{
    this->max = std::nan("0");
    this->min = std::nan("0");
}

VariablesDefinition::VariablesDefinition(const VariablesDefinition &cop) :min(cop.min), max(cop.max),
    has_min_border(cop.has_min_border), has_max_border(cop.has_max_border)
{

}
void VariablesDefinition::setMinValue(definition_area_type imin)
{
    this->min = imin;
    this->has_min_border = true;
}
void VariablesDefinition::setMaxValue(definition_area_type imax)
{
    this->max = imax;
    this->has_max_border = true;
}
VariablesDefinition getPositiveDefinition()
{
    VariablesDefinition def;
    def.setMinValue(0);
    return def;
}
definition_area_type VariablesDefinition::getMinValue()
{
    return this->min;
}
definition_area_type VariablesDefinition::getMaxValue()
{
    return this->max;
}
