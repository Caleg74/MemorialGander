#include "apparatusdata.h"

ApparatusData::ApparatusData(const int p_iApparatusId,
                             const QString& p_strNameIt,
                             const QString& p_strNameEn,
                             const QString& p_strGender)
: m_iApparatusId(p_iApparatusId)
, m_strGender(p_strGender)
{
    m_strName[ALangEn] = p_strNameEn;
    m_strName[ALangIt] = p_strNameIt;
}

int ApparatusData::getId() const
{
    return m_iApparatusId;
}

const QString& ApparatusData::getName(ApparatusLanguage_t p_eLang) const
{
    return m_strName[p_eLang];
}

const QString& ApparatusData::getGender() const
{
    return m_strGender;
}
