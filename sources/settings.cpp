#include "settings.h"
#include "ui_settings.h"
#include "constants.h"

#include <QSettings>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, &Settings::saveSettings);

    QSettings settings;
    ui->proxyEnabled->setChecked(settings.value(    SET_PROXY_ENABLED,  QVariant(PROXY_ENABLED)).toBool());
    ui->proxyHost->setText(     settings.value(     SET_PROXY_HOST,     QVariant(PROXY_HOST)).toString());
    ui->proxyPort->setText(     settings.value(     SET_PROXY_PORT,     QVariant(PROXY_PORT)).toString());
    ui->proxyUser->setText(     settings.value(     SET_PROXY_USER,     QVariant(PROXY_USER)).toString());
    ui->proxyPass->setText(     settings.value(     SET_PROXY_PASS,     QVariant(PROXY_PASS)).toString());
    ui->timeout->setText(       settings.value(     SET_TIMEOUT,        QVariant(CONNECT_TIMEOUT)).toString());
    ui->homePage->setText(      settings.value(     SET_HOME_PAGE,      QVariant(HOME_PAGE)).toString());

}

void Settings::saveSettings()
{
    QSettings settings;
    settings.setValue(SET_HOME_PAGE,        ui->homePage->text());
    settings.setValue(SET_TIMEOUT,          ui->timeout->text());
    settings.setValue(SET_PROXY_ENABLED,    ui->proxyEnabled->isChecked());
    settings.setValue(SET_PROXY_HOST,       ui->proxyHost->text());
    settings.setValue(SET_PROXY_PORT,       ui->proxyPort->text());
    settings.setValue(SET_PROXY_USER,       ui->proxyUser->text());
    settings.setValue(SET_PROXY_PASS,       ui->proxyPass->text());
}
