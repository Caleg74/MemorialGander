#ifndef APPARATUSDATA_H
#define APPARATUSDATA_H

#include <QString>

/** The Apparatus data in the DB are fix.
 *  Therefore for performance optimization it's better to get the values once at init
 *  and access them via this class.
 */
class ApparatusData
{
public:
    enum ApparatusLanguage_t
    {
        ALangIt,
        ALangEn,
        ALangMax
    };

    ApparatusData(const int p_iApparatusId,
                  const QString& p_strNameIt,
                  const QString& p_strNameEn,
                  const QString& p_strGender);

    int getId() const;

    const QString& getName(ApparatusLanguage_t p_eLang) const;

    const QString& getGender() const;
private:

    int m_iApparatusId;
    QString m_strName[ALangMax];
    QString m_strGender; ///< either "M" or "F"
};

#endif // APPARATUSDATA_H
