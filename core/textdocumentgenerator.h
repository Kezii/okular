/***************************************************************************
 *   Copyright (C) 2007 by Tobias Koenig <tokoe@kde.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _OKULAR_TEXTDOCUMENTGENERATOR_H_
#define _OKULAR_TEXTDOCUMENTGENERATOR_H_

#include <okular/core/okular_export.h>

#include <okular/core/document.h>
#include <okular/core/generator.h>

class QTextBlock;
class QTextDocument;

namespace Okular {

class TextDocumentGenerator;

class OKULAR_EXPORT TextDocumentConverter : public QObject
{
    Q_OBJECT

    public:
        /**
         * Creates a new generic converter.
         */
        TextDocumentConverter();

        /**
         * Destroys the generic converter.
         */
        ~TextDocumentConverter();

        /**
         * Returns the generated QTextDocument object.
         */
        virtual QTextDocument *convert( const QString &fileName ) = 0;

    Q_SIGNALS:
        /**
         * Adds a new link object which is located between cursorBegin and
         * cursorEnd to the generator.
         */
        void addAction( Action *link, int cursorBegin, int cursorEnd );

        /**
         * Adds a new annotation object which is located between cursorBegin and
         * cursorEnd to the generator.
         */
        void addAnnotation( Annotation *annotation, int cursorBegin, int cursorEnd );

        /**
         * Adds a new title at the given level which is located as position to the generator.
         */
        void addTitle( int level, const QString &title, const QTextBlock &position );

        /**
         * Adds a set of meta data to the generator.
         */
        void addMetaData( const QString &key, const QString &value, const QString &title );

        /**
         * This signal should be emitted whenever an error occurred in the converter.
         *
         * @param message The message which should be shown to the user.
         * @param duration The time that the message should be shown to the user.
         */
        void error( const QString &message, int duration );

        /**
         * This signal should be emitted whenever the user should be warned.
         *
         * @param message The message which should be shown to the user.
         * @param duration The time that the message should be shown to the user.
         */
        void warning( const QString &message, int duration );

        /**
         * This signal should be emitted whenever the user should be noticed.
         *
         * @param message The message which should be shown to the user.
         * @param duration The time that the message should be shown to the user.
         */
        void notice( const QString &message, int duration );

    protected:
        /**
         * This method can be used to calculate the viewport for a given text block.
         *
         * Note: This method should be called at the end of the convertion, because it
         *       triggers QTextDocument to do the layout calculation.
         */
        DocumentViewport calculateViewport( QTextDocument *document, const QTextBlock &block );

    private:
        class Private;
        Private* const d;
};

class OKULAR_EXPORT TextDocumentGenerator : public Generator
{
    friend class TextDocumentConverter;

    Q_OBJECT

    public:
        TextDocumentGenerator( TextDocumentConverter *converter );
        virtual ~TextDocumentGenerator();

        // [INHERITED] load a document and fill up the pagesVector
        bool loadDocument( const QString & fileName, QVector<Okular::Page*> & pagesVector );
        bool closeDocument();

        // [INHERITED] perform actions on document / pages
        bool canGeneratePixmap() const;
        void generatePixmap( Okular::PixmapRequest * request );

        // [INHERITED] print document using already configured kprinter
        bool print( KPrinter& printer );

        // [INHERITED] text exporting
        Okular::ExportFormat::List exportFormats() const;
        bool exportTo( const QString &fileName, const Okular::ExportFormat &format );

        const Okular::DocumentInfo* generateDocumentInfo();
        const Okular::DocumentSynopsis* generateDocumentSynopsis();

    protected:
        Okular::TextPage* textPage( Okular::Page *page );

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT( d, void addAction( Action*, int, int ) )
        Q_PRIVATE_SLOT( d, void addAnnotation( Annotation*, int, int ) )
        Q_PRIVATE_SLOT( d, void addTitle( int, const QString&, const QTextBlock& ) )
        Q_PRIVATE_SLOT( d, void addMetaData( const QString&, const QString&, const QString& ) )
};

}

#endif
