#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QDir>
#include <QMap>
#include <QProcess>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>

struct ProgramParameters {
    QWidget* widget;
    QGridLayout* layout;
    QMap<QString, QLineEdit*> lineEdits;
    QMap<QString, QPushButton*> buttons;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle("TreeSeg GUI");
    window.resize(800, 600);

    // Menu Bar
    QMenuBar *menuBar = new QMenuBar(&window);
    QMenu *programMenu = menuBar->addMenu("Programs");
    QMenu *viewMenu = menuBar->addMenu("View"); // New View menu
    QMenu *helpMenu = menuBar->addMenu("Help");
    window.setMenuBar(menuBar);

    // Central widget and main layout
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Widgets
    QLabel *label = new QLabel("Select PCD File:");
    QLineEdit *filePath = new QLineEdit();
    QPushButton *browseButton = new QPushButton("Browse");
    QWidget *paramContainer = new QWidget();
    QVBoxLayout *paramContainerLayout = new QVBoxLayout(paramContainer);
    paramContainerLayout->setContentsMargins(0, 0, 0, 0); // Reduce margins for better fit
    QPushButton *runButton = new QPushButton("Run Selected Program");
    QTextEdit *logText = new QTextEdit();
    logText->setReadOnly(true);

    // VTK Widget
    QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(&window);
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkWidget->setRenderWindow(renderWindow);
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // Logo Frame
    QFrame *logoFrame = new QFrame();
    QHBoxLayout *logoLayout = new QHBoxLayout(logoFrame);
    QLabel *logo1 = new QLabel();
    QLabel *logo2 = new QLabel();
    logo1->setPixmap(QPixmap("logo1.png").scaled(100, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo2->setPixmap(QPixmap("logo2.png").scaled(100, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    if (logo1->pixmap()->isNull()) logo1->setText("Logo 1 not found");
    if (logo2->pixmap()->isNull()) logo2->setText("Logo 2 not found");
    logoLayout->addWidget(logo1);
    logoLayout->addStretch();
    logoLayout->addWidget(logo2);
    logoFrame->setLayout(logoLayout);

    // Store parameters for each program
    QMap<QString, ProgramParameters> programParams;
    QWidget *currentParamWidget = nullptr;
    QString currentProgramName; // Track current program for run button or logging

    // Define TreeSeg programs explicitly
    QStringList treeSegPrograms = {
        "downsample",
        "findstems",
        "getcrownvolume",
        "getdtmslice",
        "leafsep",
        "nearestneighbour",
        "pcdPointTreeseg2txt",
        "pcdPointXYZRGB2txt",
        "plotcoords",
        "rxp2pcd",
        "rxp2tree",
        "segmentcrown",
        "segmentstem",
        "sepwoodleaf",
        "thin",
        "treeparams",
        "treeseg",
        "txtPointTreeseg2pcd"
    };

    // Populate program menu, only including TreeSeg programs present in directory
    QDir dir(".");
    QStringList entries = dir.entryList(QDir::NoDotAndDotDot | QDir::Files | QDir::Executable);
    QStringList programs;
    for (const QString &entry : entries) {
        QString programName = entry.split(".").first();
        if (treeSegPrograms.contains(programName)) {
            programs << programName;
        }
    }
    QAction *defaultAction = nullptr;

    for (const QString &programName : programs) {
        ProgramParameters params;
        params.widget = new QWidget(paramContainer);
        params.layout = new QGridLayout(params.widget);
        params.widget->setLayout(params.layout);

        if (programName == "downsample") {
            params.lineEdits["voxelSize"] = new QLineEdit("0.04");
            params.layout->addWidget(new QLabel("Voxel Size (m):"), 0, 0);
            params.layout->addWidget(params.lineEdits["voxelSize"], 0, 1);
        } else if (programName == "findstems") {
            params.lineEdits["smoothness"] = new QLineEdit("4");
            params.lineEdits["minDiameter"] = new QLineEdit("0.2");
            params.lineEdits["maxDiameter"] = new QLineEdit("2");
            params.lineEdits["coordsFile"] = new QLineEdit("../NOU11.coords.dat");
            params.buttons["coordsBrowse"] = new QPushButton("Browse Coords");
            params.lineEdits["sliceFile"] = new QLineEdit("../NOU11.slice.pcd");
            params.buttons["sliceBrowse"] = new QPushButton("Browse Slice");
            params.layout->addWidget(new QLabel("Smoothness (degrees):"), 0, 0);
            params.layout->addWidget(params.lineEdits["smoothness"], 0, 1);
            params.layout->addWidget(new QLabel("Min Diameter (m):"), 1, 0);
            params.layout->addWidget(params.lineEdits["minDiameter"], 1, 1);
            params.layout->addWidget(new QLabel("Max Diameter (m):"), 2, 0);
            params.layout->addWidget(params.lineEdits["maxDiameter"], 2, 1);
            params.layout->addWidget(new QLabel("Coords File:"), 3, 0);
            params.layout->addWidget(params.lineEdits["coordsFile"], 3, 1);
            params.layout->addWidget(params.buttons["coordsBrowse"], 3, 2);
            params.layout->addWidget(new QLabel("Slice PCD:"), 4, 0);
            params.layout->addWidget(params.lineEdits["sliceFile"], 4, 1);
            params.layout->addWidget(params.buttons["sliceBrowse"], 4, 2);

            // Connect browse buttons once
            QObject::connect(params.buttons["coordsBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Coords", "", "Data Files (*.dat)");
                if (!fileName.isEmpty()) params.lineEdits["coordsFile"]->setText(fileName);
            });
            QObject::connect(params.buttons["sliceBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Slice PCD", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["sliceFile"]->setText(fileName);
            });
        } else if (programName == "getcrownvolume") {
            params.lineEdits["crownHeight"] = new QLineEdit("0.66");
            params.lineEdits["stemFiles"] = new QLineEdit("../stems/NOU11.stem.*.pcd");
            params.buttons["stemBrowse"] = new QPushButton("Browse Stem Files");
            params.lineEdits["downsampleTiles"] = new QLineEdit("../NOU11.tile.downsample.*.pcd");
            params.buttons["tilesBrowse"] = new QPushButton("Browse Tiles");
            params.layout->addWidget(new QLabel("Crown Height (m):"), 0, 0);
            params.layout->addWidget(params.lineEdits["crownHeight"], 0, 1);
            params.layout->addWidget(new QLabel("Stem Files:"), 1, 0);
            params.layout->addWidget(params.lineEdits["stemFiles"], 1, 1);
            params.layout->addWidget(params.buttons["stemBrowse"], 1, 2);
            params.layout->addWidget(new QLabel("Downsample Tiles:"), 2, 0);
            params.layout->addWidget(params.lineEdits["downsampleTiles"], 2, 1);
            params.layout->addWidget(params.buttons["tilesBrowse"], 2, 2);

            // Connect browse buttons once
            QObject::connect(params.buttons["stemBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Stem PCD", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["stemFiles"]->setText(fileName);
            });
            QObject::connect(params.buttons["tilesBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Downsample Tiles", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["downsampleTiles"]->setText(fileName);
            });
        } else if (programName == "getdtmslice") {
            params.lineEdits["dtmResolution"] = new QLineEdit("2");
            params.lineEdits["dtmPercentile"] = new QLineEdit("2.5");
            params.lineEdits["dtmMinHeight"] = new QLineEdit("3");
            params.lineEdits["dtmMaxHeight"] = new QLineEdit("6");
            params.lineEdits["dtmOutput"] = new QLineEdit("NOU11.dtm.dat");
            params.lineEdits["dtmTiles"] = new QLineEdit("../NOU11.tile.downsample.*.pcd");
            params.buttons["dtmTilesBrowse"] = new QPushButton("Browse DTM Tiles");
            params.layout->addWidget(new QLabel("DTM Resolution (m):"), 0, 0);
            params.layout->addWidget(params.lineEdits["dtmResolution"], 0, 1);
            params.layout->addWidget(new QLabel("Percentile:"), 1, 0);
            params.layout->addWidget(params.lineEdits["dtmPercentile"], 1, 1);
            params.layout->addWidget(new QLabel("Min Height (m):"), 2, 0);
            params.layout->addWidget(params.lineEdits["dtmMinHeight"], 2, 1);
            params.layout->addWidget(new QLabel("Max Height (m):"), 3, 0);
            params.layout->addWidget(params.lineEdits["dtmMaxHeight"], 3, 1);
            params.layout->addWidget(new QLabel("Output File:"), 4, 0);
            params.layout->addWidget(params.lineEdits["dtmOutput"], 4, 1);
            params.layout->addWidget(new QLabel("Downsample Tiles:"), 5, 0);
            params.layout->addWidget(params.lineEdits["dtmTiles"], 5, 1);
            params.layout->addWidget(params.buttons["dtmTilesBrowse"], 5, 2);

            // Connect browse button once
            QObject::connect(params.buttons["dtmTilesBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open DTM Tiles", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["dtmTiles"]->setText(fileName);
            });
        } else if (programName == "segmentcrown") {
            params.lineEdits["smoothness"] = new QLineEdit("4");
            params.lineEdits["volumeFiles"] = new QLineEdit("../volumes/NOU11.volume.*.pcd");
            params.buttons["volumeBrowse"] = new QPushButton("Browse Volume Files");
            params.layout->addWidget(new QLabel("Smoothness (degrees):"), 0, 0);
            params.layout->addWidget(params.lineEdits["smoothness"], 0, 1);
            params.layout->addWidget(new QLabel("Volume Files:"), 1, 0);
            params.layout->addWidget(params.lineEdits["volumeFiles"], 1, 1);
            params.layout->addWidget(params.buttons["volumeBrowse"], 1, 2);

            // Connect browse button once
            QObject::connect(params.buttons["volumeBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Volume PCD", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["volumeFiles"]->setText(fileName);
            });
        } else if (programName == "segmentstem") {
            params.lineEdits["smoothness"] = new QLineEdit("4");
            params.lineEdits["stemInliers"] = new QLineEdit("../clusters/NOU11.cluster.?.pcd");
            params.buttons["stemInliersBrowse"] = new QPushButton("Browse Stem Inliers");
            params.lineEdits["downsampleTiles"] = new QLineEdit("../NOU11.tile.downsample.*.pcd");
            params.buttons["tilesBrowse"] = new QPushButton("Browse Tiles");
            params.layout->addWidget(new QLabel("Smoothness (degrees):"), 0, 0);
            params.layout->addWidget(params.lineEdits["smoothness"], 0, 1);
            params.layout->addWidget(new QLabel("Stem Inliers:"), 1, 0);
            params.layout->addWidget(params.lineEdits["stemInliers"], 1, 1);
            params.layout->addWidget(params.buttons["stemInliersBrowse"], 1, 2);
            params.layout->addWidget(new QLabel("Downsample Tiles:"), 2, 0);
            params.layout->addWidget(params.lineEdits["downsampleTiles"], 2, 1);
            params.layout->addWidget(params.buttons["tilesBrowse"], 2, 2);

            // Connect browse buttons once
            QObject::connect(params.buttons["stemInliersBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Stem Inliers", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["stemInliers"]->setText(fileName);
            });
            QObject::connect(params.buttons["tilesBrowse"], &QPushButton::clicked, [=, &window]() {
                QString fileName = QFileDialog::getOpenFileName(&window, "Open Downsample Tiles", "", "PCD Files (*.pcd)");
                if (!fileName.isEmpty()) params.lineEdits["downsampleTiles"]->setText(fileName);
            });
        } // Other programs (e.g., leafsep, nearestneighbour, etc.) will have an empty parameter widget

        params.widget->hide(); // Hide by default
        programParams[programName] = params;

        QAction *action = programMenu->addAction(programName);
        if (programName == "downsample") defaultAction = action;

        QObject::connect(action, &QAction::triggered, [=, &logText, &paramContainerLayout, &currentParamWidget, &currentProgramName]() {
            // Insert new message at the top
            logText->moveCursor(QTextCursor::Start);
            logText->insertPlainText("Selecting program: " + programName + "\n");

            // Remove and hide current parameter widget if exists
            if (currentParamWidget) {
                paramContainerLayout->removeWidget(currentParamWidget);
                currentParamWidget->hide();
            }

            // Add and show new parameter widget
            currentParamWidget = programParams[programName].widget;
            paramContainerLayout->addWidget(currentParamWidget);
            currentParamWidget->show();

            currentProgramName = programName;
            // Insert second message at the top
            logText->moveCursor(QTextCursor::Start);
            logText->insertPlainText("Parameters updated for " + programName + "\n");
            logText->ensureCursorVisible(); // Ensure the top (newest message) is visible
        });
    }

    if (defaultAction) defaultAction->trigger(); // Select downsample by default

    // View Menu - PCL Viewer Action
    QAction *viewAction = viewMenu->addAction("View Selected PCD");
    QObject::connect(viewAction, &QAction::triggered, [&window, filePath, &logText]() {
        QString pcdFile = filePath->text().trimmed();
        if (pcdFile.isEmpty()) {
            logText->moveCursor(QTextCursor::Start);
            logText->insertPlainText("Error: No PCD file selected for viewing.\n");
            logText->ensureCursorVisible();
            QMessageBox::warning(&window, "No File Selected", "Please select a PCD file first.");
            return;
        }

        if (!QFile::exists(pcdFile)) {
            logText->moveCursor(QTextCursor::Start);
            logText->insertPlainText("Error: PCD file does not exist: " + pcdFile + "\n");
            logText->ensureCursorVisible();
            QMessageBox::warning(&window, "File Not Found", "The selected PCD file does not exist.");
            return;
        }

        QProcess *viewerProcess = new QProcess(&window);
        QStringList args;
        args << pcdFile;
        viewerProcess->start("pcl_viewer", args);

        if (!viewerProcess->waitForStarted(3000)) { // Wait up to 3 seconds
            logText->moveCursor(QTextCursor::Start);
            logText->insertPlainText("Error: Failed to start pcl_viewer. Ensure it's installed and in PATH.\n");
            logText->ensureCursorVisible();
            QMessageBox::warning(&window, "Viewer Error", "Failed to start pcl_viewer. Check installation and PATH.");
            delete viewerProcess;
            return;
        }

        logText->moveCursor(QTextCursor::Start);
        logText->insertPlainText("Started pcl_viewer for: " + pcdFile + "\n");
        logText->ensureCursorVisible();
    });

    // Help Menu
    QAction *aboutAction = helpMenu->addAction("About");
    QObject::connect(aboutAction, &QAction::triggered, [&]() {
        QMessageBox::about(&window, "About TreeSeg GUI",
                           "TreeSeg GUI\n"
                           "Version: 1.0\n"
                           "Developed for processing point cloud data (PCD) files.\n"
                           "Supports multiple TreeSeg programs.\n"
                           "© 2025 Your Name/Organization");
    });

    // Layout
    mainLayout->addWidget(label);
    mainLayout->addWidget(filePath);
    mainLayout->addWidget(browseButton);
    mainLayout->addWidget(paramContainer);
    mainLayout->addWidget(runButton);
    mainLayout->addWidget(logText);
    mainLayout->addWidget(vtkWidget);
    mainLayout->addWidget(logoFrame);
    centralWidget->setLayout(mainLayout);
    window.setCentralWidget(centralWidget);

    // Connections
    QObject::connect(browseButton, &QPushButton::clicked, [&window, filePath]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Open PCD", "", "PCD Files (*.pcd)");
        if (!fileName.isEmpty()) filePath->setText(fileName);
    });

    QObject::connect(runButton, &QPushButton::clicked, [&programMenu, &logText]() {
        // Note: This re-triggers the selection as in original; add execution logic here if needed
        QAction *selectedAction = programMenu->activeAction();
        if (selectedAction) {
            selectedAction->trigger();
        }
    });

    window.show();
    return app.exec();
}

