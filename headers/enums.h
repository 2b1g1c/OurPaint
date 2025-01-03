#ifndef OURPAINT_ENUMS_H
#define OURPAINT_ENUMS_H
enum Element {
    ET_POINT, ET_SECTION, ET_CIRCLE
};
enum Requirement {
    ET_POINTSECTIONDIST,
    ET_POINTONSECTION,
    ET_POINTPOINTDIST,
    ET_POINTONPOINT,
    ET_SECTIONCIRCLEDIST,
    ET_SECTIONONCIRCLE,
    ET_SECTIONINCIRCLE,
    ET_SECTIONSECTIONPARALLEL,
    ET_SECTIONSECTIONPERPENDICULAR,
    ET_SECTIONSECTIONANGEL,
    ET_POINTONCIRCLE,
    ET_POINTINOBJECT
};
#endif //OURPAINT_ENUMS_H
