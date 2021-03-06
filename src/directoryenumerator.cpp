/*
 * libnitroshareutil
 * Copyright (C) 2013  Nathan Osman
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 */

#include <QDir>
#include <QFileInfoList>
#include <QList>
#include <QStack>

#include <nitroshare/util/fileheaderlist.h>
#include <nitroshare/util/parameterbuilder.h>
#include "directoryenumerator.h"

using namespace NitroShare::Util;

bool DirectoryEnumerator::cancelable() const
{
    return true;
}

void DirectoryEnumerator::run(const QVariantMap & parameters)
{
    FileHeaderListPointer headers(new FileHeaderList);
    qint64 size = 0;

    QStack<FileHeaderPointer> stack;
    stack.push(FileHeaderPointer(new FileHeader(parameters["directory"].toString())));

    while(stack.count())
    {
        if(wasCanceled())
        {
            emit canceled();
            return;
        }

        FileHeaderPointer tos = stack.pop();
        QFileInfoList contents = QDir(tos->absoluteFilename())
            .entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

        foreach(QFileInfo info, contents)
        {
            FileHeaderPointer header(new FileHeader(info,
                QString("%1/%3").arg(tos->relativeFilename()).arg(info.fileName())));

            if(info.isDir())
                stack.push(header);
            else
            {
                headers->addHeader(header);
                size += info.size();
            }
        }
    }

    emit completed(ParameterBuilder() << "headers" << QVariant::fromValue(headers)
                                      << "size"    << size);
}
