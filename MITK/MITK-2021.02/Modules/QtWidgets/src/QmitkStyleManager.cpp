/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include <QmitkStyleManager.h>

#include <mitkLogMacros.h>

#include <QApplication>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QRegularExpression>

namespace
{
  QString ParseColor(const QString &subject, const QString &pattern, const QString &fallback)
  {
    QRegularExpression re(pattern, QRegularExpression::CaseInsensitiveOption);
    auto match = re.match(subject);

    return match.hasMatch()
      ? match.captured(1)
      : fallback;
  }
}

QIcon QmitkStyleManager::ThemeIcon(const QByteArray &originalSVG)
{
  auto styleSheet = qApp->styleSheet();

  if (styleSheet.isEmpty())
    return QPixmap::fromImage(QImage::fromData(originalSVG));

  auto iconColor = ParseColor(styleSheet,
    QStringLiteral("iconColor\\s*[=:]\\s*(#[0-9a-f]{6})"),
    QStringLiteral("#000000"));

  auto iconAccentColor = ParseColor(styleSheet,
    QStringLiteral("iconAccentColor\\s*[=:]\\s*(#[0-9a-f]{6})"),
    QStringLiteral("#ffffff"));

  auto themedSVG = QString(originalSVG).replace(QStringLiteral("#00ff00"), iconColor, Qt::CaseInsensitive);
  themedSVG = themedSVG.replace(QStringLiteral("#ff00ff"), iconAccentColor, Qt::CaseInsensitive);

  return QPixmap::fromImage(QImage::fromData(themedSVG.toLatin1()));
}

QIcon QmitkStyleManager::ThemeIcon(const QString &resourcePath)
{
  QFile resourceFile(resourcePath);

  if (resourceFile.open(QIODevice::ReadOnly))
  {
    auto originalSVG = resourceFile.readAll();
    return ThemeIcon(originalSVG);
  }

  MITK_WARN << "Could not read " << resourcePath.toStdString();
  return QIcon();
}
