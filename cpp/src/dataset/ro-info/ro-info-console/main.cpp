
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "ro-info/ro-info.h"

#include <QDebug>
#include <QUrl>

#include "kans.h"

USING_KANS(DSM)


int main(int argc, char **argv)
{
 RO_Info ro;
 ro.set_article_url(QUrl("http://www.felsemiotica.org/site/wp-content/uploads/2014/10/Horn-Laurence-R.-and-Ward-Gregory-Ed.-The-Handbook-of-Pragmatics.pdf"));
 ro.set_main_repo_url(QUrl("https://github.com/scignscape/HandbookPragmatics"));
 ro.set_main_executable_path(TARGETS_DIR "/dsmain_console.exe");
 ro.set_local_article_path(AR_ROOT_DIR "/cpp/about/about-files/main.pdf");
 ro.set_main_project_path(AR_ROOT_DIR "cpp/projects/qt/qtm/both/dataset/dsmain/dsmain-console");

 ro.parse_kai_signatures_from_file("ScignStage_Ling_Dialog",
   AR_ROOT_DIR "/cpp/kph/dataset/raw-multi/t1.kph.sigs");

 ro.parse_kai_signatures_from_file("Lexpair_Dialog",
   AR_ROOT_DIR "/cpp/kph/dataset/raw-multi/t1.kph.lexpair.sigs");

 qDebug() << ro.main_executable_path();
 qDebug() << ro.local_article_path();
 qDebug() << ro.main_project_path();
 qDebug() << '\n' << ro.kai_signatures()["ScignStage_Ling_Dialog"].join("  ");

 qDebug() << '\n' << ro.kai_signatures()["Lexpair_Dialog"].join("  ");

 return 0;
}
