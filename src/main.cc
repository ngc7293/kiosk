#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineParser>

#include "app.h"

void buildArgParser(QCommandLineParser& parser)
{
    QCommandLineOption urls(QStringList() << "u" << "urls", QObject::tr("File with URLs to load"), "urls");
    QCommandLineOption start(QStringList() << "s" << "start", QObject::tr("Start cycling on launch"));
    QCommandLineOption fullscreen(QStringList() << "f" << "fullscreen", QObject::tr("Launch fullscreen"));
    QCommandLineOption css(QStringList() << "css", QObject::tr("Stylesheet to inject"), "css");

    parser.addOptions(QList<QCommandLineOption>() << urls << start << fullscreen << css);
}

int main(int argc, char* argv[])
{
    QApplication qapp(argc, argv);
    QCommandLineParser parser;
    buildArgParser(parser);
    parser.process(qapp);

    App app(parser);
    app.show();
    return qapp.exec();
}