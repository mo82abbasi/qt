/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEANIMATION_H
#define QDECLARATIVEANIMATION_H

#include "qdeclarativetransition_p.h"
#include "qdeclarativestate_p.h"
#include <QtGui/qvector3d.h>

#include <qdeclarativepropertyvaluesource.h>
#include <qdeclarative.h>
#include <qdeclarativescriptstring.h>

#include <QtCore/qvariant.h>
#include <QtCore/qeasingcurve.h>
#include <QtCore/QAbstractAnimation>
#include <QtGui/qcolor.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

class QDeclarativeAbstractAnimationPrivate;
class QDeclarativeAnimationGroup;
class Q_AUTOTEST_EXPORT QDeclarativeAbstractAnimation : public QObject, public QDeclarativePropertyValueSource, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeAbstractAnimation)

    Q_INTERFACES(QDeclarativeParserStatus)
    Q_INTERFACES(QDeclarativePropertyValueSource)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool alwaysRunToEnd READ alwaysRunToEnd WRITE setAlwaysRunToEnd NOTIFY alwaysRunToEndChanged())
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_CLASSINFO("DefaultMethod", "start()")

public:
    QDeclarativeAbstractAnimation(QObject *parent=0);
    virtual ~QDeclarativeAbstractAnimation();

    bool isRunning() const;
    void setRunning(bool);
    bool isPaused() const;
    void setPaused(bool);
    bool alwaysRunToEnd() const;
    void setAlwaysRunToEnd(bool);
    bool repeat() const;
    void setRepeat(bool);

    int currentTime();
    void setCurrentTime(int);

    QDeclarativeAnimationGroup *group() const;
    void setGroup(QDeclarativeAnimationGroup *);

    void setDefaultTarget(const QDeclarativeProperty &);
    void setDisableUserControl();

    void classBegin();
    void componentComplete();

Q_SIGNALS:
    void started();
    void completed();
    void runningChanged(bool);
    void pausedChanged(bool);
    void repeatChanged(bool);
    void alwaysRunToEndChanged(bool);

public Q_SLOTS:
    void restart();
    void start();
    void pause();
    void resume();
    void stop();
    void complete();

protected:
    QDeclarativeAbstractAnimation(QDeclarativeAbstractAnimationPrivate &dd, QObject *parent);

public:
    enum TransitionDirection { Forward, Backward };
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation() = 0;

private Q_SLOTS:
    void timelineComplete();

private:
    virtual void setTarget(const QDeclarativeProperty &);
};

class QDeclarativePauseAnimationPrivate;
class Q_AUTOTEST_EXPORT QDeclarativePauseAnimation : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePauseAnimation)

    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)

public:
    QDeclarativePauseAnimation(QObject *parent=0);
    virtual ~QDeclarativePauseAnimation();

    int duration() const;
    void setDuration(int);

Q_SIGNALS:
    void durationChanged(int);

protected:
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativeScriptActionPrivate;
class QDeclarativeScriptAction : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeScriptAction)

    Q_PROPERTY(QDeclarativeScriptString script READ script WRITE setScript)
    Q_PROPERTY(QString stateChangeScriptName READ stateChangeScriptName WRITE setStateChangeScriptName)

public:
    QDeclarativeScriptAction(QObject *parent=0);
    virtual ~QDeclarativeScriptAction();

    QDeclarativeScriptString script() const;
    void setScript(const QDeclarativeScriptString &);

    QString stateChangeScriptName() const;
    void setStateChangeScriptName(const QString &);

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativePropertyActionPrivate;
class QDeclarativePropertyAction : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePropertyAction)

    Q_PROPERTY(QObject *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY targetChanged)
    Q_PROPERTY(QString properties READ properties WRITE setProperties NOTIFY propertiesChanged)
    Q_PROPERTY(QDeclarativeListProperty<QObject> targets READ targets)
    Q_PROPERTY(QDeclarativeListProperty<QObject> exclude READ exclude)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    QDeclarativePropertyAction(QObject *parent=0);
    virtual ~QDeclarativePropertyAction();

    QObject *target() const;
    void setTarget(QObject *);

    QString property() const;
    void setProperty(const QString &);

    QString properties() const;
    void setProperties(const QString &);

    QDeclarativeListProperty<QObject> targets();
    QDeclarativeListProperty<QObject> exclude();

    QVariant value() const;
    void setValue(const QVariant &);

Q_SIGNALS:
    void valueChanged(const QVariant &);
    void propertiesChanged(const QString &);
    void targetChanged(QObject *, const QString &);

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativeItem;
class QDeclarativeParentActionPrivate;
class QDeclarativeParentAction : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeParentAction)

    Q_PROPERTY(QDeclarativeItem *target READ object WRITE setObject)
    Q_PROPERTY(QDeclarativeItem *parent READ parent WRITE setParent) //### newParent

public:
    QDeclarativeParentAction(QObject *parent=0);
    virtual ~QDeclarativeParentAction();

    QDeclarativeItem *object() const;
    void setObject(QDeclarativeItem *);

    QDeclarativeItem *parent() const;
    void setParent(QDeclarativeItem *);

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativePropertyAnimationPrivate;
class Q_AUTOTEST_EXPORT QDeclarativePropertyAnimation : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePropertyAnimation)

    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QVariant from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QVariant to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(QEasingCurve easing READ easing WRITE setEasing NOTIFY easingChanged)
    Q_PROPERTY(QObject *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY targetChanged)
    Q_PROPERTY(QString properties READ properties WRITE setProperties NOTIFY propertiesChanged)
    Q_PROPERTY(QDeclarativeListProperty<QObject> targets READ targets)
    Q_PROPERTY(QDeclarativeListProperty<QObject> exclude READ exclude)

public:
    QDeclarativePropertyAnimation(QObject *parent=0);
    virtual ~QDeclarativePropertyAnimation();

    int duration() const;
    void setDuration(int);

    QVariant from() const;
    void setFrom(const QVariant &);

    QVariant to() const;
    void setTo(const QVariant &);

    QEasingCurve easing() const;
    void setEasing(const QEasingCurve &);

    QObject *target() const;
    void setTarget(QObject *);

    QString property() const;
    void setProperty(const QString &);

    QString properties() const;
    void setProperties(const QString &);

    QDeclarativeListProperty<QObject> targets();
    QDeclarativeListProperty<QObject> exclude();

protected:
    QDeclarativePropertyAnimation(QDeclarativePropertyAnimationPrivate &dd, QObject *parent);
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();

Q_SIGNALS:
    void durationChanged(int);
    void fromChanged(QVariant);
    void toChanged(QVariant);
    void easingChanged(const QEasingCurve &);
    void propertiesChanged(const QString &);
    void targetChanged(QObject *, const QString &);
};

class Q_AUTOTEST_EXPORT QDeclarativeColorAnimation : public QDeclarativePropertyAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePropertyAnimation)
    Q_PROPERTY(QColor from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QColor to READ to WRITE setTo NOTIFY toChanged)

public:
    QDeclarativeColorAnimation(QObject *parent=0);
    virtual ~QDeclarativeColorAnimation();

    QColor from() const;
    void setFrom(const QColor &);

    QColor to() const;
    void setTo(const QColor &);
};

class Q_AUTOTEST_EXPORT QDeclarativeNumberAnimation : public QDeclarativePropertyAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePropertyAnimation)

    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)

public:
    QDeclarativeNumberAnimation(QObject *parent=0);
    virtual ~QDeclarativeNumberAnimation();

    qreal from() const;
    void setFrom(qreal);

    qreal to() const;
    void setTo(qreal);
};

class Q_AUTOTEST_EXPORT QDeclarativeVector3dAnimation : public QDeclarativePropertyAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativePropertyAnimation)

    Q_PROPERTY(QVector3D from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QVector3D to READ to WRITE setTo NOTIFY toChanged)

public:
    QDeclarativeVector3dAnimation(QObject *parent=0);
    virtual ~QDeclarativeVector3dAnimation();

    QVector3D from() const;
    void setFrom(QVector3D);

    QVector3D to() const;
    void setTo(QVector3D);
};

class QDeclarativeRotationAnimationPrivate;
class Q_AUTOTEST_EXPORT QDeclarativeRotationAnimation : public QDeclarativePropertyAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeRotationAnimation)
    Q_ENUMS(RotationDirection)

    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(RotationDirection direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    QDeclarativeRotationAnimation(QObject *parent=0);
    virtual ~QDeclarativeRotationAnimation();

    qreal from() const;
    void setFrom(qreal);

    qreal to() const;
    void setTo(qreal);

    enum RotationDirection { Numerical, Shortest, Clockwise, Counterclockwise };
    RotationDirection direction() const;
    void setDirection(RotationDirection direction);

Q_SIGNALS:
    void directionChanged();
};

class QDeclarativeAnimationGroupPrivate;
class QDeclarativeAnimationGroup : public QDeclarativeAbstractAnimation
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeAnimationGroup)

    Q_CLASSINFO("DefaultProperty", "animations")
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations READ animations)

public:
    QDeclarativeAnimationGroup(QObject *parent);
    virtual ~QDeclarativeAnimationGroup();

    QDeclarativeListProperty<QDeclarativeAbstractAnimation> animations();
    friend class QDeclarativeAbstractAnimation;

protected:
    QDeclarativeAnimationGroup(QDeclarativeAnimationGroupPrivate &dd, QObject *parent);
};

class QDeclarativeSequentialAnimation : public QDeclarativeAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeAnimationGroup)

public:
    QDeclarativeSequentialAnimation(QObject *parent=0);
    virtual ~QDeclarativeSequentialAnimation();

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativeParallelAnimation : public QDeclarativeAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeAnimationGroup)

public:
    QDeclarativeParallelAnimation(QObject *parent=0);
    virtual ~QDeclarativeParallelAnimation();

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

class QDeclarativeParentAnimationPrivate;
class QDeclarativeParentAnimation : public QDeclarativeAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeclarativeParentAnimation)

    Q_PROPERTY(QDeclarativeItem *target READ target WRITE setTarget)
    Q_PROPERTY(QDeclarativeItem *newParent READ newParent WRITE setNewParent)
    Q_PROPERTY(QDeclarativeItem *via READ via WRITE setVia)

public:
    QDeclarativeParentAnimation(QObject *parent=0);
    virtual ~QDeclarativeParentAnimation();

    QDeclarativeItem *target() const;
    void setTarget(QDeclarativeItem *);

    QDeclarativeItem *newParent() const;
    void setNewParent(QDeclarativeItem *);

    QDeclarativeItem *via() const;
    void setVia(QDeclarativeItem *);

protected:
    virtual void transition(QDeclarativeStateActions &actions,
                            QDeclarativeProperties &modified,
                            TransitionDirection direction);
    virtual QAbstractAnimation *qtAnimation();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeAbstractAnimation)
QML_DECLARE_TYPE(QDeclarativePauseAnimation)
QML_DECLARE_TYPE(QDeclarativeScriptAction)
QML_DECLARE_TYPE(QDeclarativePropertyAction)
QML_DECLARE_TYPE(QDeclarativeParentAction)
QML_DECLARE_TYPE(QDeclarativePropertyAnimation)
QML_DECLARE_TYPE(QDeclarativeColorAnimation)
QML_DECLARE_TYPE(QDeclarativeNumberAnimation)
QML_DECLARE_TYPE(QDeclarativeSequentialAnimation)
QML_DECLARE_TYPE(QDeclarativeParallelAnimation)
QML_DECLARE_TYPE(QDeclarativeVector3dAnimation)
QML_DECLARE_TYPE(QDeclarativeRotationAnimation)
QML_DECLARE_TYPE(QDeclarativeParentAnimation)

QT_END_HEADER

#endif // QDECLARATIVEANIMATION_H