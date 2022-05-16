#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QtGlobal>
#include <QList>
#include <QColor>
#include "models/wcode.h"
#include <QMap>

class MainViewModel
{
public:

    struct DoWorkModel{
        int i;
    };

    struct DoWorkRModel{
        QString txt;
        QMap<QString, Wcode> wcodes;
    };

    struct ListItemChangedModel{
        QString selectedItemKey;
    };

    struct ListItemChangedModelR{
        Wcode wcode;
    };

    struct GenerateR{
        QString code;
    };
};


#endif // MAINVIEWMODEL_H
