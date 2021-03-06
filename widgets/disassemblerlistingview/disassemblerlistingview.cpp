#include "disassemblerlistingview.h"
#include "../../themeprovider.h"
#include "../../redasmsettings.h"
#include <QScrollBar>

DisassemblerListingView::DisassemblerListingView(QWidget *parent): QSplitter(parent), m_disassembler(NULL)
{
    this->setOrientation(Qt::Horizontal);
    this->setStyleSheet("QSplitter::handle { background-color: " + THEME_VALUE_COLOR("seek") + "; }");

    m_disassemblertextview = new DisassemblerTextView(this);
    m_disassemblercolumnview = new DisassemblerColumnView(this);

    m_disassemblertextview->setFont(REDasmSettings::font());
    m_disassemblercolumnview->setFont(m_disassemblertextview->font()); // Apply same font

    connect(m_disassemblertextview->verticalScrollBar(), &QScrollBar::valueChanged, this, [&](int) { this->renderArrows(); });

    this->addWidget(m_disassemblercolumnview);
    this->addWidget(m_disassemblertextview);

    this->setStretchFactor(0, 2);
    this->setStretchFactor(1, 10);
    this->setHandleWidth(4);
}

DisassemblerColumnView *DisassemblerListingView::columnView() { return m_disassemblercolumnview; }
DisassemblerTextView *DisassemblerListingView::textView() { return m_disassemblertextview; }

void DisassemblerListingView::setDisassembler(const REDasm::DisassemblerPtr& disassembler)
{
    m_disassembler = disassembler;
    m_disassemblercolumnview->setDisassembler(m_disassembler);
    m_disassemblertextview->setDisassembler(m_disassembler);

    REDasm::ListingDocument& document = m_disassembler->document();

    EVENT_CONNECT(document->cursor(), positionChanged, this, [&]() {
        if(m_disassembler->busy())
            return;

        QMetaObject::invokeMethod(m_disassemblercolumnview, "update", Qt::QueuedConnection);
    });

    EVENT_CONNECT(m_disassembler, busyChanged, this, [&]() {
        if(m_disassembler->busy())
            return;

        QMetaObject::invokeMethod(this, "renderArrows", Qt::QueuedConnection);
    });
}

void DisassemblerListingView::renderArrows()
{
    if(m_disassembler->busy())
        return;

    m_disassemblercolumnview->renderArrows(m_disassemblertextview->firstVisibleLine(), m_disassemblertextview->visibleLines());
}
