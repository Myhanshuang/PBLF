#include "ClickablePixmapItem.h"
#include <QGraphicsSceneMouseEvent>

// Constructor implementation (if needed)
ClickablePixmapItem::ClickablePixmapItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent) {
    // Optionally, set any default properties or behaviors here
}

// Override mousePressEvent to emit the `clicked` signal
void ClickablePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit clicked(); // Emit the signal when the item is clicked
    QGraphicsPixmapItem::mousePressEvent(event); // Call the base class implementation
}

// Add a method to scale the image dynamically
void ClickablePixmapItem::resizeTo(const QSize& targetSize) {
    if (!pixmap().isNull()) {
        setPixmap(pixmap().scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
