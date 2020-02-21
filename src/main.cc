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
    QCommandLineOption ipc(QStringList() << "ipc", QObject::tr("Enable the IPC server"));

    parser.addOptions(QList<QCommandLineOption>() << ipc << urls << start << fullscreen << css);
    parser.addHelpOption();
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