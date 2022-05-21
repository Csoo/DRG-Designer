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
        enum State {
            CONTINUE = -1,
            STOP,
            PAUSE
        };
        Q_ENUM(CodeType)
        Q_ENUM(State)
};

#endif // ENUM_HPP
