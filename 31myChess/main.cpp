#include "chess.h"
#include "chessform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    chess w;
//    w.show();
    ChessForm w;
    w.show();

    return a.exec();
}
