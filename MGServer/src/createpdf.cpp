#include "createpdf.h"
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QHash>
#include <algorithm>
#include <cmath>
#include <utility>
#include "dbinterface.h"

namespace {
constexpr qreal sponsorFooterTop = 750.0;
// Allow for the widest table and its borders when scaling both pages.
constexpr qreal allroundLayoutWidth = 790.0;

QString compactScore(float score)
{
    QString formatted = QString::number(score, 'f', 3);
    while (formatted.endsWith(u'0') && !formatted.endsWith(u".0"))
        formatted.chop(1);
    return formatted;
}
}

CreatePdf::CreatePdf()
 : QObject()
// , m_doc()
 , m_printer(QPrinter::PrinterResolution)
 , m_cRank()
 , m_iPageNumber(0)
{
}

void CreatePdf::retriveGymnastList()
{
    m_cRank.createLists();
}

void CreatePdf::PrintRanking()
{
    retriveGymnastList();
    // 1st update the scores
    m_cRank.updateAllroundScores();

    const int edition = dbInterface::Instance()->getCurrentEvent();
    const bool withLogo = true;

    QString strFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    strFileName += QStringLiteral("/MemorialGander%1.pdf").arg(edition);

    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setOutputFileName(strFileName);

    if (! m_painter.begin(&m_printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    m_iPageNumber = 1;
    PrintMenAllround(withLogo);

    if (! m_printer.newPage()) {
        qWarning("failed in flushing page to disk, disk full?");
    }

    PrintWomenAllround(withLogo);

    if (! m_printer.newPage()) {
        qWarning("failed in flushing page to disk, disk full?");
    }

    PrintAllSingleApparatusMen(withLogo);

    PrintAllSingleApparatusWomen(withLogo);

    m_painter.end();

    qDebug() << "File created in " << strFileName;
}

void CreatePdf::PrintLastApparatusStartOrder()
{
    retriveGymnastList();
    m_cRank.updateAllroundScores();

    const int edition = dbInterface::Instance()->getCurrentEvent();
    const QString strFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                              + QStringLiteral("/MemorialGander%1_FinalApparatusStartingOrder.pdf").arg(edition);

    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setOutputFileName(strFileName);

    if (! m_painter.begin(&m_printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    m_iPageNumber = 1;
    PrintLastApparatusOrder(true);

    m_painter.end();

    qDebug() << "File created in " << strFileName;
}

void CreatePdf::PrintLastApparatusOrder(bool p_bHFImages)
{
    struct Finalist {
        QString name;
        QString gender;
        QString apparatus;
    };

    QList<QStringList> subscriptions;
    dbInterface::Instance()->retrieveGymnastSubscriptionList(subscriptions);
    QHash<int, int> assignedApparatusByAthlete;
    for (const QStringList &subscription : std::as_const(subscriptions)) {
        if (subscription.size() >= 7)
            assignedApparatusByAthlete.insert(subscription.at(0).toInt(), subscription.at(6).toInt());
    }

    const auto finalistsInStartingOrder = [&](const QList<AthleteData> &ranking, const QString &gender) {
        constexpr qsizetype finalistsPerCategory = 6;
        const qsizetype count = qMin(finalistsPerCategory, ranking.size());
        auto *apparatusList = ApparatusList::Instance();

        QList<Finalist> finalists;
        finalists.reserve(count);
        for (qsizetype position = count; position > 0; --position) {
            AthleteData gymnast = ranking.at(position - 1);
            const int apparatusId = assignedApparatusByAthlete.value(gymnast.getAthleteId());
            QString apparatus = apparatusList->apparatusName(apparatusId, ApparatusData::ALangEn);
            if (!apparatus.isEmpty() && apparatusList->getApparatusId(apparatus, gender) != apparatusId)
                apparatus.clear();

            finalists.append({gymnast.getNameFull().trimmed(), gender, apparatus});
        }
        return finalists;
    };

    const auto women = finalistsInStartingOrder(*m_cRank.getAllroundWomenList(), QStringLiteral("F"));
    const auto men = finalistsInStartingOrder(*m_cRank.getAllroundMenList(), QStringLiteral("M"));
    QList<Finalist> combined;
    combined.reserve(women.size() + men.size());
    for (qsizetype position = 0; position < qMax(women.size(), men.size()); ++position) {
        if (position < women.size())
            combined.append(women.at(position));
        if (position < men.size())
            combined.append(men.at(position));
    }

    QString html;
    QTextStream out(&html);
    const auto printTable = [&](const char *title, const QList<Finalist> &finalists, bool showGender) {
        out << "<h2 style=\"font-size:10pt; margin-top:0px; margin-bottom:6px;\">"
            << title << "</h2>"
            << "<table border=\"1\" cellpadding=\"3\" width=\"100%\" "
               "style=\"border-collapse:collapse; border-color:black;"
            << (showGender ? " margin-bottom:16px;" : " font-size:8pt;") << "\">"
            << "<tr><th width=\"" << (showGender ? 9 : 7) << "%\">#</th>";
        if (showGender)
            out << "<th width=\"10%\">M/F</th>";
        out << "<th align=\"left\">Name</th>"
            << "<th align=\"left\" width=\"" << (showGender ? 36 : 41) << "%\">Apparatus</th></tr>";

        qsizetype order = 1;
        for (const Finalist &finalist : finalists) {
            out << "<tr><td align=\"center\">" << order++ << "</td>";
            if (showGender)
                out << "<td align=\"center\">" << finalist.gender << "</td>";
            out << "<td>" << finalist.name.toHtmlEscaped() << "</td>"
                << "<td>" << finalist.apparatus.toHtmlEscaped() << "</td></tr>";
        }
        out << "</table>";
    };

    out << "<h1 style=\"font-size:15pt; margin-top:0px; margin-bottom:8px;\">Final Apparatus</h1>"
        << "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" style=\"margin-top:12px;\"><tr>"
        << "<td colspan=\"3\">";
    printTable("Gymnast Starting Order", combined, true);
    out << "</td></tr><tr><td width=\"48%\" valign=\"top\">";
    printTable("Women's Starting Order", women, false);
    out << "</td><td width=\"4%\"></td><td width=\"48%\" valign=\"top\">";
    printTable("Men's Starting Order", men, false);
    out << "</td></tr></table>";
    out.flush();

    AddHeader(p_bHFImages);
    AddFooter(p_bHFImages);
    AddPageNumber();

    constexpr qreal horizontalMargin = 20;
    constexpr qreal contentTop = 105;
    QTextDocument document;
    document.setDefaultFont(QFont(QStringLiteral("Arial"), 9));
    document.setDocumentMargin(0);
    document.setHtml(html);
    document.setTextWidth(m_painter.viewport().width() - 2 * horizontalMargin);

    m_painter.save();
    m_painter.translate(horizontalMargin, contentTop);
    document.drawContents(&m_painter);
    m_painter.restore();
}

void CreatePdf::PrintMenAllround(bool p_bHFImages)
{
    QString html;
    QTextStream out(&html);

    PrintHeader(p_bHFImages, out);
    PrintMenTableTitle(out);
    PrintMenTableBody(out);
    PrintFooter(p_bHFImages, out);

    out.flush();
    DrawAllroundHtml(html);
}

void CreatePdf::PrintWomenAllround(bool p_bHFImages)
{
    QString html;
    QTextStream out(&html);

    PrintHeader(p_bHFImages, out);
    PrintWomenTableTitle(out);
    PrintWomenTableBody(out);
    PrintFooter(p_bHFImages, out);

    out.flush();
    DrawAllroundHtml(html);
}

void CreatePdf::PrintAllSingleApparatusMen(bool p_bHFImages)
{
    for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AMApparatusMax; apparatus++)
    {
        QString html;
        QTextStream out(&html);

        PrintHeader(p_bHFImages, out);
        m_cRank.updateSingleScores(static_cast<ApparatusList::EApparatusMen>(apparatus));
        PrintSingleMApparatusTableTitle(out, static_cast<ApparatusList::EApparatusMen>(apparatus));
        PrintSingleApparatusTableBody(out, *m_cRank.getSingleMenList());
        PrintFooter(p_bHFImages, out);

        out.flush();
        DrawHtml(html);

        if (! m_printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
        }
    }
}

void CreatePdf::PrintAllSingleApparatusWomen(bool p_bHFImages)
{
    for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AWApparatusMax; apparatus++)
    {
        QString html;
        QTextStream out(&html);

        PrintHeader(p_bHFImages, out);
        m_cRank.updateSingleScores(static_cast<ApparatusList::EApparatusWomen>(apparatus));
        PrintSingleWApparatusTableTitle(out, static_cast<ApparatusList::EApparatusWomen>(apparatus));
        PrintSingleApparatusTableBody(out, *m_cRank.getSingleWomenList());
        PrintFooter(p_bHFImages, out);

        out.flush();
        DrawHtml(html);

        if (apparatus != (ApparatusList::AWApparatusMax-1))
        {
            if (! m_printer.newPage()) {
                qWarning("failed in flushing page to disk, disk full?");
            }
        }
    }
}

void CreatePdf::PrintHeader(bool p_bHFImages, QTextStream& out)
{
    // Add header
    QFile fileIn(":/pdf/IN_NoHeader.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strHeader(&fileIn);
    out << strHeader.readAll(); // copy to output file
    fileIn.close();

    AddHeader(p_bHFImages);
}

void CreatePdf::PrintFooter(bool p_bHFImages, QTextStream& out)
{
    QFile fileIn(":/pdf/IN_NoFooter.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strFooter(&fileIn);
    out << strFooter.readAll();
    fileIn.close();

    AddFooter(p_bHFImages);
    AddPageNumber();
}

void CreatePdf::PrintMenTableTitle(QTextStream& out)
{
    QFile fileIn(":/pdf/IN_MenTableTitle.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintMenTableBody(QTextStream& out)
{
    PrintAllroundTableBody(out, *m_cRank.getAllroundMenList(), ApparatusList::AMApparatusMax);
}

void CreatePdf::PrintWomenTableTitle(QTextStream& out)
{
    QFile fileIn(":/pdf/IN_WomenTableTitle.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintWomenTableBody(QTextStream& out)
{
    PrintAllroundTableBody(out, *m_cRank.getAllroundWomenList(), ApparatusList::AWApparatusMax);
}

void CreatePdf::PrintAllroundTableBody(QTextStream& out, const QList<AthleteData>& gymnasts,
                                     int apparatusEnd)
{
    const auto numericScore = [](const QString &score) {
        const QString value = score.trimmed();
        return value == QStringLiteral("-") ? QStringLiteral("0.000") : value;
    };
    const auto detailLine = [](const QString &left, const QString &rightLabel, float rightScore) {
        const QString value = compactScore(rightScore);
        const QString right = value == QStringLiteral("0.0") || value == QStringLiteral("-0.0")
                                  ? QString{} : rightLabel + value;
        return left.leftJustified(9, u'\u00a0') + right.leftJustified(8, u'\u00a0');
    };
    for (const auto &gymnast : gymnasts) {
        out << "<tr><td align=\"center\" style=\"font-size:10px;font-weight:bold;\">"
            << gymnast.getRank() << "</td>"
            << "<td style=\"font-size:10px;font-weight:bold;\">"
            << gymnast.getNameFull().trimmed().toHtmlEscaped()
            << " (" << gymnast.getNationShort().toHtmlEscaped() << ")</td>";

        // Total first, followed by each apparatus in the existing column order.
        for (int apparatus = ApparatusList::AGTotalScore; apparatus < apparatusEnd; ++apparatus) {
            const QString final = gymnast.getFinalScore(apparatus).trimmed();
            out << "<td align=\"center\"><span style=\"font-size:11px;font-weight:bold;\">"
                << final << "</span>";
            if (final != QStringLiteral("-")) {
                const bool isApparatus = apparatus != ApparatusList::AGTotalScore;
                const QString difficulty = QStringLiteral("D=") + numericScore(gymnast.getDifficultyScore(apparatus));
                const QString execution = QStringLiteral("E=") + numericScore(gymnast.getExecutionScore(apparatus));
                out << "<br/><span style=\"font-family:Consolas;font-size:9px;font-weight:normal;\">";
                if (isApparatus) {
                    out << detailLine(difficulty, QStringLiteral("B="), gymnast.getBonusScore(apparatus))
                        << "<br/>"
                        << detailLine(execution, QStringLiteral("P="),
                                      -std::abs(gymnast.getPenaltyScore(apparatus).toFloat()));
                } else {
                    out << difficulty << "<br/>" << execution;
                }
                out << "</span>";
            }
            out << "</td>";
        }
        out << "</tr>" << Qt::endl;
    }
}

void CreatePdf::PrintSingleMApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusMen p_eApparatus)
{
    QFile fileIn;

    switch (p_eApparatus)
    {
    case ApparatusList::EApparatusMen::AMFloor:
        fileIn.setFileName(":/pdf/IN_Men_Floor.html");
        break;

    case ApparatusList::EApparatusMen::AMHorizontalBar:
        fileIn.setFileName(":/pdf/IN_Men_HorizontalBar.html");
        break;

    case ApparatusList::EApparatusMen::AMParallelBars:
        fileIn.setFileName(":/pdf/IN_Men_ParallelBars.html");
        break;

    case ApparatusList::EApparatusMen::AMPommelHorse:
        fileIn.setFileName(":/pdf/IN_Men_PommelHorse.html");
        break;

    case ApparatusList::EApparatusMen::AMRings:
        fileIn.setFileName(":/pdf/IN_Men_Rings.html");
        break;

    case ApparatusList::EApparatusMen::AMVault:
        fileIn.setFileName(":/pdf/IN_Men_Vault.html");
        break;

    default:
        qDebug() << "Unable to find table title for this apparatus " << p_eApparatus;

    }

    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintSingleApparatusTableBody(QTextStream& out, const QList<SingleMWData>& gymnasts)
{
    constexpr QChar noteSpace{u'\u00a0'};
    const QString bonusPrefix = QStringLiteral("\u00a0\u00a0B=");
    const QString penaltyPrefix = QStringLiteral("\u00a0\u00a0P=-");
    const auto formatPenalty = [](const SingleMWData &gymnast) {
        QString penalty = gymnast.getPenaltyScore();
        if (penalty.endsWith(u"00"))
            penalty.chop(2);
        return penalty;
    };

    // Size the optional notes from the scores printed on this apparatus page.
    constexpr qsizetype minimumValueWidth = 3; // 0.0
    qsizetype bonusValueWidth = minimumValueWidth;
    qsizetype penaltyValueWidth = minimumValueWidth;
    for (const auto &gymnast : gymnasts) {
        if (gymnast.getFinalScore() == QStringLiteral("0.000"))
            continue;
        bonusValueWidth = std::max(bonusValueWidth, compactScore(gymnast.getBonusScore()).size());
        penaltyValueWidth = std::max(penaltyValueWidth, formatPenalty(gymnast).size());
    }

    for (const auto& gymnast : gymnasts)
    {
        // Skip if final score is 0
        const QString finalScore = gymnast.getFinalScore();
        if (finalScore == QStringLiteral("0.000"))
            continue;

        const QString penalty = formatPenalty(gymnast);
        const QString bonus = compactScore(gymnast.getBonusScore());
        const QString bonusNote = bonus == QStringLiteral("0.0")
                                      ? QString{} : bonusPrefix + bonus;
        const QString penaltyNote = penalty == QStringLiteral("0.0")
                                        ? QString{} : penaltyPrefix + penalty;

        out << "\t<tr>\n"
            << "\t  <th rowspan=\"2\" valign=\"middle\">" << gymnast.getRank() << "</th>\n"
            << "\t  <th rowspan=\"2\" valign=\"middle\" align=\"left\" width=\"180\">"
            << gymnast.getNameFull() << "(" << gymnast.getNation() << ")</th>\n"
            << "\t  <th align=\"center\">" << finalScore << "</th>\n"
            << "\t</tr>\n"
            << "\t<tr>\n"
            << "\t  <td align=\"center\"><small><span style=\"font-family:Consolas;\">D="
            << gymnast.getDifficultyScore()
            << QStringLiteral("\u00a0\u00a0E=") << gymnast.getExecutionScore()
            << bonusNote.leftJustified(bonusPrefix.size() + bonusValueWidth, noteSpace)
            << penaltyNote.leftJustified(penaltyPrefix.size() + penaltyValueWidth, noteSpace)
            << "</span></small></td>\n"
            << "\t</tr>\n";
    }
}

void CreatePdf::PrintSingleWApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusWomen p_eApparatus)
{
    QFile fileIn;

    switch (p_eApparatus)
    {
    case ApparatusList::EApparatusWomen::AWFloor:
        fileIn.setFileName(":/pdf/IN_Women_Floor.html");
        break;

    case ApparatusList::EApparatusWomen::AWBalanceBeam:
        fileIn.setFileName(":/pdf/IN_Women_BalanceBeam.html");
        break;

    case ApparatusList::EApparatusWomen::AWUnevenBars:
        fileIn.setFileName(":/pdf/IN_Women_UnevenBars.html");
        break;

    case ApparatusList::EApparatusWomen::AWVault:
        fileIn.setFileName(":/pdf/IN_Women_Vault.html");
        break;

    default:
        qDebug() << "Unable to find table title for this apparatus " << p_eApparatus;

    }

    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }

    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::AddHeader(bool p_bHFImages)
{
    if (p_bHFImages)
    {
        QRectF target(20.0, 10.0, 496.0/3, 239.0/3);
        QRectF source(0.0, 0.0, 496.0, 239.0);
        QImage image(":/pdf/IN_Header_LogoMG.png");
        m_painter.drawImage(target, image, source);
        return;
    }

    m_painter.save();
    m_painter.setFont(QFont(QStringLiteral("Arial"), 12, QFont::Bold));
    m_painter.setPen(Qt::black);
    m_painter.drawText(QPointF(20.0, 38.0), QStringLiteral("Memorial Arturo Gander %1")
                           .arg(dbInterface::Instance()->getCurrentEvent()));
    m_painter.restore();
}

void CreatePdf::AddFooter(bool p_bHFImages)
{
    if (p_bHFImages)
    {
        QRectF target(20.0, sponsorFooterTop, 2480.0/3, 283.0/3);
        QRectF source(0.0, 0.0, 2480.0, 283.0);
        QImage image(":/pdf/IN_Footer_Sponsors.png");
        m_painter.drawImage(target, image, source);  // Paint the mask onto the image
    }
}


void CreatePdf::AddPageNumber()
{
    int x=m_printer.pageRect(QPrinter::Point).width()-60;
    int y=m_printer.pageRect(QPrinter::Point).height()-30;
    m_painter.setFont({"Arial", 10});
    m_painter.setPen({0, 0, 128, 128});
    m_painter.drawText(x, y, "Page " + QString::number(m_iPageNumber));

    // inc for next page
    m_iPageNumber++;
}

void CreatePdf::DrawAllroundHtml(QString html)
{
    // The legacy <font size> wrapper overrides nested CSS font sizes in Qt.
    // Remove it only on the two overall pages, which define their own typography.
    html.remove(QStringLiteral("<font face=\"arial\" size=\"2\">"));
    html.remove(QStringLiteral("</font>"));
    html.remove(QStringLiteral("<br>"));
    constexpr qreal left = 20;
    constexpr qreal top = 105;
    constexpr qreal footerGap = 12;
    DrawHtml(html, QRectF(left, top, m_painter.viewport().width() - 2 * left,
                          sponsorFooterTop - top - footerGap));
}

void CreatePdf::DrawHtml(const QString& html, const QRectF& contentArea)
{
    static const QStringList apparatusImageNames = {
        QStringLiteral("BalanceBeam_bw.png"),
        QStringLiteral("Floor_bw.png"),
        QStringLiteral("HorizontalBar_bw.png"),
        QStringLiteral("ParallelBars_bw.png"),
        QStringLiteral("PommelHorse_bw.png"),
        QStringLiteral("Rings_bw.png"),
        QStringLiteral("UnevenBars_bw.png"),
        QStringLiteral("Vault_bw.png")
    };

    QTextDocument doc;
    for (const QString& imageName : apparatusImageNames) {
        const QImage image(QStringLiteral(":/pdf/") + imageName);
        if (image.isNull()) {
            qWarning() << "Unable to load PDF apparatus image" << imageName;
            continue;
        }

        doc.addResource(QTextDocument::ImageResource,
                        QUrl(QStringLiteral("qrc:/pdf/") + imageName),
                        image);
    }

    QString resourceHtml = html;
    resourceHtml.replace(QStringLiteral("../MGServer/pdf/"),
                         QStringLiteral("qrc:/pdf/"));
    doc.setHtml(resourceHtml);
    if (contentArea.isEmpty()) {
        doc.drawContents(&m_painter);
        return;
    }

    doc.setDocumentMargin(0);
    const QSizeF size = doc.size();
    const qreal scale = std::min({1.0, contentArea.width() / std::max(allroundLayoutWidth, size.width()),
                                  contentArea.height() / size.height()});
    m_painter.save();
    m_painter.translate(contentArea.topLeft());
    m_painter.scale(scale, scale);
    doc.drawContents(&m_painter);
    m_painter.restore();
}
