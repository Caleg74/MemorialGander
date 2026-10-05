#ifndef COMBOBOXMODEL_H
#define COMBOBOXMODEL_H

#include <QObject>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

class ComboBoxModel : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(QStringList comboList READ comboList WRITE setComboList NOTIFY comboListChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit ComboBoxModel(QObject *parent = nullptr);
    ComboBoxModel(const QStringList &list,int count, QObject *parent = 0);

    QStringList comboList() const;
    void setComboList(const QStringList &comboList);

    int count() const;

    Q_INVOKABLE void addElement(const QString &element);
    Q_INVOKABLE void removeElement(int index);

signals:

    void comboListChanged();
    void countChanged();

private:

    QStringList m_comboList;
};

#endif // COMBOBOXMODEL_H
