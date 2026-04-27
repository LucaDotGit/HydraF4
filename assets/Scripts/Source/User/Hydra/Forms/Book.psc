Scriptname Hydra:Forms:Book Const Hidden Native

;/
	Provides functions for books.
/;

bool Function GetHasBeenRead(Book akBook) Global Native
Function SetHasBeenRead(Book akBook, bool abValue) Global Native

int Function GetTextOffsetX(Book akBook) Global Native
Function SetTextOffsetX(Book akBook, int aiValue) Global Native

int Function GetTextOffsetY(Book akBook) Global Native
Function SetTextOffsetY(Book akBook, int aiValue) Global Native

Static Function GetInventoryModel(Book akBook) Global Native
Function SetInventoryModel(Book akBook, Static akValue) Global Native


bool Function HasFlag(Book akBook, int aiFlag) Global Native
int Function GetFlags(Book akBook) Global Native
Function SetFlags(Book akBook, int aiFlags) Global Native
Function SetFlag(Book akBook, int aiFlag, bool abSet) Global Native

;/
	Book Flags:
/;

int Function GetFlag_HasBeenRead() Global Native
int Function GetFlag_DisallowTaking() Global Native
