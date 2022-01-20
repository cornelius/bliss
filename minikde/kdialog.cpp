#include <KDialog>

#include <QBoxLayout>

KDialog::KDialog(QWidget *parent)
  : QDialog(parent), mOkButton(0), mCancelButton(0)
{
  mButtonBox = new QDialogButtonBox;
  connect(mButtonBox, SIGNAL(accepted()), SLOT(accept()));
  connect(mButtonBox, SIGNAL(rejected()), SLOT(reject()));
}

void KDialog::setMainWidget(QWidget *mainWidget)
{
  QBoxLayout *topLayout = new QVBoxLayout(this);
  topLayout->addWidget(mainWidget);
  topLayout->addWidget(mButtonBox);
}

QPushButton *KDialog::button(Button button)
{
  if (button == Ok) {
    return mOkButton;
  } else if (button == Cancel) {
    return mCancelButton;
  }
  return 0;
}

void KDialog::setCaption(const QString &title)
{
  setWindowTitle(title);
}

void KDialog::setButtons(int buttons)
{
  if (buttons & Ok) {
    mOkButton = mButtonBox->addButton(QDialogButtonBox::Ok);
  }
  if (buttons & Cancel) {
    mCancelButton = mButtonBox->addButton(QDialogButtonBox::Cancel);
  }
}
