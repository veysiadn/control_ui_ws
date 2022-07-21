#include <QApplication>
#include "../include/gui_pkg/main_window.hpp"
#include "../include/gui_pkg/endoscope_viewer.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  MainWindow w(argc, argv);
  // EndoscopeViewer gui_endoscope_viewer;
  // gui_endoscope_viewer.setWindowTitle("Spine Robot Endoscope Viewer");
  w.setWindowTitle("EtherCAT Control UI by Veysi ADIN ©");
  w.show();
  // gui_endoscope_viewer.show();
  int result = app.exec();
  return result;
}
