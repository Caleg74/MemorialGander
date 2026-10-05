#include "comboboxmodel.h"

ComboBoxModel::ComboBoxModel(QObject *parent) : QObject(parent) {}

ComboBoxModel::ComboBoxModel(const QStringList &list, int count, QObject *parent)
    : QObject(parent), m_comboList(list)
{
    Q_UNUSED(count); // Compatibility overload: the list determines its size.
}

QStringList ComboBoxModel::comboList() const { return m_comboList; }
int ComboBoxModel::count() const { return static_cast<int>(m_comboList.size()); }

void ComboBoxModel::setComboList(const QStringList &list)
{
    if (m_comboList == list)
        return;
    const bool sizeChanged = m_comboList.size() != list.size();
    m_comboList = list;
    emit comboListChanged();
    if (sizeChanged)
        emit countChanged();
}

void ComboBoxModel::addElement(const QString &element)
{
    m_comboList.append(element);
    emit comboListChanged();
    emit countChanged();
}

void ComboBoxModel::removeElement(int index)
{
    if (index < 0 || index >= m_comboList.size())
        return;
    m_comboList.removeAt(index);
    emit comboListChanged();
    emit countChanged();
}
