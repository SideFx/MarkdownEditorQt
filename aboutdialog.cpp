/////////////////////////////////////////////////////////////////////////////
// Name:        aboutdialog.cpp
// Purpose:     Create About dialog
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-11-05
/////////////////////////////////////////////////////////////////////////////

#include <QPushButton>
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    setAutoFillBackground(true);
    setStyleSheet("QDialog { background-color: palette(window); }");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Close"));
    ui->lbl_text1->setText(TxtAbout);
    adjustSize();
    setFixedSize(size());
}

AboutDialog::~AboutDialog() {
    delete ui;
}
