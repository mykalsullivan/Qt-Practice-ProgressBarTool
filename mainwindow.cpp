#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QProgressBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, [=]() {
        m_Running = true;
        qDebug() << "Progress bar started";

        ui->startButton->setText("Working...");
        ui->startButton->setDisabled(true);

        ui->infoLabel->setText("");
        makeProgress();
    });

    connect(ui->resetButton, &QPushButton::clicked, [=]() {
        qDebug() << "User attempted progress bar reset";

        QMessageBox confirmDialog;
        confirmDialog.setWindowTitle("Confirm Reset");
        confirmDialog.setText("Are you sure you want to reset the progress bar?");
        confirmDialog.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        confirmDialog.setDefaultButton(QMessageBox::Yes);
        auto result = confirmDialog.exec();

        switch (result) {
        case QMessageBox::Yes:
            m_Progress = 0;
            qDebug() << "Progress bar reset";

            ui->resetButton->setDisabled(true);
            ui->startButton->setEnabled(true);
            ui->progressBar->setValue(0);
            ui->infoLabel->setText("Progress bar reset.");
            break;
        case QMessageBox::Cancel:
            qDebug() << "User cancelled reset";
            break;
        default:
            break;
        }
    });

    connect(ui->progressBar, &QProgressBar::valueChanged, [=]() {
        if (ui->progressBar->value() == 100) {
            m_Running = false;
            qDebug() << "Done";

            ui->startButton->setText("Start");
            ui->startButton->setDisabled(true);
            ui->resetButton->setEnabled(true);
            ui->infoLabel->setText("Done.");
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeProgress()
{
    while (m_Running) {
        m_Progress++;
        ui->progressBar->setValue(m_Progress);

        if (m_Running) {
            qDebug() << "Making progress: " << QString::number(m_Progress);
            QThread::msleep(50);
        }
    }
    QMessageBox::information(this, "Done", "Progress complete!");
}
