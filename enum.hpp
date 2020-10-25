#ifndef ENUM_HPP
#define ENUM_HPP

#include <QObject>

class Type : public QObject
{
    Q_OBJECT

    public:
        Type() : QObject() {}

        enum CodeType {
            DRG_SYS = 0,
            DRG_CAPTER,
            DRG,
            DRG_TYPE,
            ICD11,
            BNO10,
            AXIS
        };
        Q_ENUMS(CodeType)
};

#endif // ENUM_HPP
