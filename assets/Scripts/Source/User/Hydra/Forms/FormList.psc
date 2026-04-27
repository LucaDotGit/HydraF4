Scriptname Hydra:Forms:FormList Const Hidden Native

;/
	Provides functions for FormLists.

	Notes:
	- The script forms are save-baked while the editor forms are not.
/;

int Function GetFormCount(FormList akFormList) Global Native
int Function GetEditorFormCount(FormList akFormList) Global Native
int Function GetScriptFormCount(FormList akFormList) Global Native

Form[] Function GetAllForms(FormList akFormList) Global Native

Form[] Function GetEditorForms(FormList akFormList) Global Native
Function SetEditorForms(FormList akFormList, Form[] akValues) Global Native

Form[] Function GetScriptForms(FormList akFormList) Global Native
Function SetScriptForms(FormList akFormList, Form[] akValues) Global Native

bool Function ContainsForm(FormList akFormList, Form akValue) Global Native
bool Function ContainsEditorForm(FormList akFormList, Form akValue) Global Native
bool Function ContainsScriptForm(FormList akFormList, Form akValue) Global Native

int Function GetFormIndex(FormList akFormList, Form akValue) Global Native
int Function GetEditorFormIndex(FormList akFormList, Form akValue) Global Native
int Function GetScriptFormIndex(FormList akFormList, Form akValue) Global Native

Form Function GetNthForm(FormList akFormList, int aiIndex) Global Native
bool Function SetNthForm(FormList akFormList, int aiIndex, Form akValue) Global Native
bool Function RemoveNthForm(FormList akFormList, int aiIndex) Global Native

Form Function GetNthEditorForm(FormList akFormList, int aiIndex) Global Native
bool Function SetNthEditorForm(FormList akFormList, int aiIndex, Form akValue) Global Native
bool Function RemoveNthEditorForm(FormList akFormList, int aiIndex) Global Native

Form Function GetNthScriptForm(FormList akFormList, int aiIndex) Global Native
bool Function SetNthScriptForm(FormList akFormList, int aiIndex, Form akValue) Global Native
bool Function RemoveNthScriptForm(FormList akFormList, int aiIndex) Global Native

bool Function AddEditorForm(FormList akFormList, Form akValue) Global Native
bool Function RemoveEditorForm(FormList akFormList, Form akValue) Global Native

bool Function AddScriptForm(FormList akFormList, Form akValue) Global Native
bool Function RemoveScriptForm(FormList akFormList, Form akValue) Global Native

Function AddEditorForms(FormList akFormList, Form[] akValues) Global Native
Function RemoveEditorForms(FormList akFormList, Form[] akValues) Global Native

Function AddScriptForms(FormList akFormList, Form[] akValues) Global Native
Function RemoveScriptForms(FormList akFormList, Form[] akValues) Global Native

Function ClearForms(FormList akFormList) Global Native
Function ClearEditorForms(FormList akFormList) Global Native
Function ClearScriptForms(FormList akFormList) Global Native
