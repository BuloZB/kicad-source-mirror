/////////////////////////////////////////////////////////////////////////////
// Name:        dialog_pad_edit.h
// Purpose:     
// Author:      jean-pierre Charras
// Modified by: 
// Created:     28/02/2006 13:34:48
// RCS-ID:      
// Copyright:   License GNU
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 28/02/2006 13:34:48

#ifndef _DIALOG_PAD_EDIT_H_
#define _DIALOG_PAD_EDIT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "dialog_pad_edit.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxBoxSizer;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10000
#define ID_TEXTCTRL_PADNUM 10001
#define ID_TEXTCTRL_NETNAME 10002
#define ID_RADIOBOX_DRILL_SHAPE 10019
#define ID_LISTBOX_ORIENT_PAD 10003
#define ID_LISTBOX_SHAPE_PAD 10004
#define ID_LISTBOX_TYPE_PAD 10005
#define ID_CHECKBOX 10006
#define ID_CHECKBOX1 10007
#define ID_CHECKBOX2 10008
#define ID_CHECKBOX3 10009
#define ID_CHECKBOX4 10010
#define ID_CHECKBOX5 10011
#define ID_CHECKBOX6 10012
#define ID_CHECKBOX7 10013
#define ID_CHECKBOX8 10014
#define ID_CHECKBOX9 10015
#define ID_CHECKBOX10 10016
#define ID_CHECKBOX11 10017
#define ID_CHECKBOX12 10018
#define SYMBOL_WINEDA_PADPROPERTIESFRAME_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WINEDA_PADPROPERTIESFRAME_TITLE _("Pad properties")
#define SYMBOL_WINEDA_PADPROPERTIESFRAME_IDNAME ID_DIALOG
#define SYMBOL_WINEDA_PADPROPERTIESFRAME_SIZE wxSize(400, 300)
#define SYMBOL_WINEDA_PADPROPERTIESFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WinEDA_PadPropertiesFrame class declaration
 */

class WinEDA_PadPropertiesFrame: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WinEDA_PadPropertiesFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WinEDA_PadPropertiesFrame( );
    WinEDA_PadPropertiesFrame( WinEDA_BasePcbFrame* parent,
            D_PAD * Pad,wxDC * DC,
            wxWindowID id = SYMBOL_WINEDA_PADPROPERTIESFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_PADPROPERTIESFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_PADPROPERTIESFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_PADPROPERTIESFRAME_SIZE, long style = SYMBOL_WINEDA_PADPROPERTIESFRAME_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WINEDA_PADPROPERTIESFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_PADPROPERTIESFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_PADPROPERTIESFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_PADPROPERTIESFRAME_SIZE, long style = SYMBOL_WINEDA_PADPROPERTIESFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin WinEDA_PadPropertiesFrame event handler declarations

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_RADIOBOX_DRILL_SHAPE
    void OnRadioboxDrillShapeSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_LISTBOX_ORIENT_PAD
    void OnListboxOrientPadSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_LISTBOX_SHAPE_PAD
    void OnListboxShapePadSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_LISTBOX_TYPE_PAD
    void OnListboxTypePadSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

////@end WinEDA_PadPropertiesFrame event handler declarations

////@begin WinEDA_PadPropertiesFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WinEDA_PadPropertiesFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	void PadPropertiesAccept(wxCommandEvent& event);
	void PadTypeSelected(void);
	void PadTypeSelectedEvent(wxCommandEvent& event);
	void PadOrientEvent(wxCommandEvent& event);
	void SetPadLayersList(long layer_mask);
	void SetOthersControls(void);

////@begin WinEDA_PadPropertiesFrame member variables
    wxBoxSizer* m_LeftBoxSizer;
    wxTextCtrl* m_PadNumCtrl;
    wxTextCtrl* m_PadNetNameCtrl;
    wxBoxSizer* m_PadPositionBoxSizer;
    wxBoxSizer* m_DrillShapeBoxSizer;
    wxRadioBox* m_DrillShapeCtrl;
    wxBoxSizer* m_PadOptSizer;
    wxRadioBox* m_PadOrient;
    wxRadioBox* m_PadShape;
    wxRadioBox* m_PadType;
    wxCheckBox* m_PadLayerCu;
    wxCheckBox* m_PadLayerCmp;
    wxCheckBox* m_PadLayerAdhCmp;
    wxCheckBox* m_PadLayerAdhCu;
    wxCheckBox* m_PadLayerPateCmp;
    wxCheckBox* m_PadLayerPateCu;
    wxCheckBox* m_PadLayerSilkCmp;
    wxCheckBox* m_PadLayerSilkCu;
    wxCheckBox* m_PadLayerMaskCmp;
    wxCheckBox* m_PadLayerMaskCu;
    wxCheckBox* m_PadLayerECO1;
    wxCheckBox* m_PadLayerECO2;
    wxCheckBox* m_PadLayerDraft;
////@end WinEDA_PadPropertiesFrame member variables
	WinEDA_BasePcbFrame * m_Parent;
	wxDC * m_DC;
	MODULE * m_Module;
	D_PAD * CurrentPad;

	WinEDA_SizeCtrl * m_PadSizeCtrl;
	WinEDA_PositionCtrl * m_PadPositionCtrl;
	WinEDA_SizeCtrl * m_PadDeltaSizeCtrl;
	WinEDA_SizeCtrl * m_PadOffsetCtrl;
	WinEDA_SizeCtrl * m_PadDrillCtrl;
	WinEDA_ValueCtrl * m_PadOrientCtrl;	// Pour orientation a 0.1 degre pres

};

#endif
    // _DIALOG_PAD_EDIT_H_
