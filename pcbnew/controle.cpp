/********************************************************/
/* Routines generales de gestion des commandes usuelles */
/********************************************************/

/* fichier controle.cpp */

/*
 *  Routines d'affichage grille, Boite de coordonnees, Curseurs, marqueurs ...
 */

#include "fctsys.h"
#include "gr_basic.h"

#include "common.h"
#include "pcbnew.h"

#include "id.h"
#include "protos.h"
#include "collectors.h"

/* Routines Locales : */

/* Variables Locales */

/**********************************/
void RemoteCommand( const char* cmdline )
/**********************************/

/* Read a remote command send by eeschema via a socket,
 *  port KICAD_PCB_PORT_SERVICE_NUMBER (currently 4242)
 */
{
    char             line[1024];
    wxString         msg;
    char*            idcmd;
    char*            text;
    WinEDA_PcbFrame* frame = EDA_Appl->m_PcbFrame;
    MODULE*          module = 0;

    strncpy( line, cmdline, sizeof(line) - 1 );
    msg = CONV_FROM_UTF8( line );

    idcmd = strtok( line, " \n\r" );
    text  = strtok( NULL, " \n\r" );
    if( (idcmd == NULL) || (text == NULL) )
        return;

    if( strcmp( idcmd, "$PART:" ) == 0 )
    {
        msg    = CONV_FROM_UTF8( text );
        
        module = ReturnModule( frame->m_Pcb, msg );
        
        msg.Printf( _( "Locate module %s %s" ), msg.GetData(),
                   module ? wxT( "Ok" ) : wxT( "not found" ) );
        
        frame->Affiche_Message( msg );
        if( module )
        {
            wxClientDC dc( frame->DrawPanel );

            frame->DrawPanel->PrepareGraphicContext( &dc );
            frame->DrawPanel->CursorOff( &dc );
            frame->GetScreen()->m_Curseur = module->m_Pos;
            frame->DrawPanel->CursorOn( &dc );
        }
    }

    if( idcmd && strcmp( idcmd, "$PIN:" ) == 0 )
    {
        wxString pinName, modName;
        D_PAD*   pad = NULL;
        int      netcode = -1;
        
        pinName = CONV_FROM_UTF8( text );
        
        text = strtok( NULL, " \n\r" );
        if( text && strcmp( text, "$PART:" ) == 0 )
            text = strtok( NULL, "\n\r" );

        wxClientDC dc( frame->DrawPanel );

        frame->DrawPanel->PrepareGraphicContext( &dc );

        modName = CONV_FROM_UTF8( text );
        module  = ReturnModule( frame->m_Pcb, modName );
        if( module )
            pad = ReturnPad( module, pinName );
        
        if( pad )
            netcode = pad->m_NetCode;
        
        if( netcode > 0 )    /* hightlighted the net selected net*/
        {
            if( g_HightLigt_Status )        /* erase the old hightlighted net */
                frame->Hight_Light( &dc );
            
            g_HightLigth_NetCode = netcode;
            frame->Hight_Light( &dc );      /* hightlighted the new one */
            
            frame->DrawPanel->CursorOff( &dc );
            frame->GetScreen()->m_Curseur = pad->m_Pos;
            frame->DrawPanel->CursorOn( &dc );
        }

        if( module == NULL )
            msg.Printf( _( "module %s not found" ), text );
        else if( pad == NULL )
            msg.Printf( _( "Pin %s (module %s) not found" ), pinName.GetData(), modName.GetData() );
        else
            msg.Printf( _( "Locate Pin %s (module %s)" ), pinName.GetData(), modName.GetData() );
        frame->Affiche_Message( msg );
    }
        
    if( module )    // if found, center the module on screen.
        frame->Recadre_Trace( false );
}


/***********************************************************************/
BOARD_ITEM* WinEDA_BasePcbFrame::PcbGeneralLocateAndDisplay()
/***********************************************************************/

/* Search an item under the mouse cursor.
 *  items are searched first on the current working layer.
 *  if nothing found, an item will be searched without layer restriction
 */
{
    BOARD_ITEM* item;
    
#if defined(DEBUG)

    // test scaffolding for Scan():
    m_Collector->Scan( m_Pcb, 
                       GetScreen()->RefPos(true), 
                       
                       // these two are inadequate, because the layer support
                       // in Kicad is not elegant or easily understood.
                       // The final solution will be a new class COLLECTORS_GUIDE!
                       GetScreen()->m_Active_Layer,
                       g_DesignSettings.GetVisibleLayers() 
                       );

    // use only the first one collected for now.
    item = (*m_Collector)[0];  // grab first one, may be NULL
    
    std::cout << "collected " << m_Collector->GetCount() << '\n';       // debugging only
    
    if( item )
    {
        item->Display_Infos( this );
        
        // debugging: print out the collected items, showing their order too.
        for( unsigned i=0; i<m_Collector->GetCount();  ++i )
            (*m_Collector)[i]->Show( 0, std::cout ); 
    }
    return item;
    
#else
    
    item = Locate( CURSEUR_OFF_GRILLE, GetScreen()->m_Active_Layer );
    if( item == NULL )
        item = Locate( CURSEUR_OFF_GRILLE, -1 );
    return item;
#endif    
}


/****************************************************************/
void WinEDA_BasePcbFrame::GeneralControle( wxDC* DC, wxPoint Mouse )
/*****************************************************************/
{
    int     ll;
    wxSize  delta;
    int     zoom = GetScreen()->GetZoom();
    wxPoint curpos, oldpos;
    int     hotkey = 0;

    ActiveScreen = GetScreen();

    // Save the board after the time out :
    int CurrentTime = time( NULL );
    if( !GetScreen()->IsModify() || GetScreen()->IsSave() )
    {   
        /* If no change, reset the time out */
        g_SaveTime = CurrentTime;
    }

    if( (CurrentTime - g_SaveTime) > g_TimeOut )
    {
        wxString tmpFileName = GetScreen()->m_FileName;
        wxString filename    = g_SaveFileName + PcbExtBuffer;
        bool     flgmodify   = GetScreen()->IsModify();

        ( (WinEDA_PcbFrame*) this )->SavePcbFile( filename );

        if( flgmodify ) // Set the flags m_Modify cleared by SavePcbFile()
        {
            GetScreen()->SetModify();
            GetScreen()->SetSave(); // Set the flags m_FlagSave cleared by SetModify()
        }
        GetScreen()->m_FileName = tmpFileName;
        SetTitle( GetScreen()->m_FileName );
    }

    curpos = DrawPanel->CursorRealPosition( Mouse );
    oldpos = GetScreen()->m_Curseur;

    delta.x = (int) round( (double) GetScreen()->GetGrid().x / zoom );
    delta.y = (int) round( (double) GetScreen()->GetGrid().y / zoom );
    if( delta.x <= 0 )
        delta.x = 1;
    if( delta.y <= 0 )
        delta.y = 1;

    switch( g_KeyPressed )
    {
    case EDA_PANNING_UP_KEY:
        OnZoom( ID_ZOOM_PANNING_UP );
        curpos = m_CurrentScreen->m_Curseur;
        break;

    case EDA_PANNING_DOWN_KEY:
        OnZoom( ID_ZOOM_PANNING_DOWN );
        curpos = m_CurrentScreen->m_Curseur;
        break;

    case EDA_PANNING_LEFT_KEY:
        OnZoom( ID_ZOOM_PANNING_LEFT );
        curpos = m_CurrentScreen->m_Curseur;
        break;

    case EDA_PANNING_RIGHT_KEY:
        OnZoom( ID_ZOOM_PANNING_RIGHT );
        curpos = m_CurrentScreen->m_Curseur;
        break;

	case EDA_ZOOM_IN_FROM_MOUSE:
        OnZoom( ID_ZOOM_PLUS_KEY );
        oldpos = curpos = GetScreen()->m_Curseur;
        break;

	case EDA_ZOOM_OUT_FROM_MOUSE:
        OnZoom( ID_ZOOM_MOINS_KEY );
        oldpos = curpos = GetScreen()->m_Curseur;
        break;

	case EDA_ZOOM_CENTER_FROM_MOUSE:
        OnZoom( ID_ZOOM_CENTER_KEY );
        oldpos = curpos = GetScreen()->m_Curseur;
        break;

    case WXK_NUMPAD8:       /* Deplacement curseur vers le haut */
    case WXK_UP:
        Mouse.y -= delta.y;
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD2:       /* Deplacement curseur vers le bas */
    case WXK_DOWN:
        Mouse.y += delta.y;
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD4:       /* Deplacement curseur vers la gauche */
    case WXK_LEFT:
        Mouse.x -= delta.x;
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD6:      /* Deplacement curseur vers la droite */
    case WXK_RIGHT:
        Mouse.x += delta.x;
        DrawPanel->MouseTo( Mouse );
        break;

    default:
        hotkey = g_KeyPressed;
        break;
    }

    /* Put cursor in new position, according to the zoom keys (if any) */
    GetScreen()->m_Curseur = curpos;

    /* Put cursor on grid or a pad centre if requested
     * But if the tool DELETE is active the cursor is left off grid
     * this is better to reach items to delete off grid
     */
    D_PAD* pad;
    bool   keep_on_grid = TRUE;
    if( m_ID_current_state == ID_PCB_DELETE_ITEM_BUTT )
        keep_on_grid = FALSE;
    
    /* Cursor is left off grid if no block in progress and no moving object */
    if( GetScreen()->BlockLocate.m_State != STATE_NO_BLOCK )
        keep_on_grid = TRUE;
    
    EDA_BaseStruct* DrawStruct = GetScreen()->GetCurItem();
    if( DrawStruct && DrawStruct->m_Flags )
        keep_on_grid = TRUE;

    switch( g_MagneticPadOption )
    {
    case capture_cursor_in_track_tool:
    case capture_always:
        pad = Locate_Any_Pad( m_Pcb, CURSEUR_OFF_GRILLE, TRUE );
        if( (m_ID_current_state != ID_TRACK_BUTT )
           && (g_MagneticPadOption == capture_cursor_in_track_tool) )
            pad = NULL;
        if( keep_on_grid )
        {
            if( pad )       // Put cursor on the pad
                GetScreen()->m_Curseur = curpos = pad->m_Pos;
            else
                // Put cursor on grid
                PutOnGrid( &GetScreen()->m_Curseur );
        }
        break;

    case no_effect:
    default:

        // If we are not in delete function, put cursor on grid
        if( keep_on_grid )
            PutOnGrid( &GetScreen()->m_Curseur );
        break;
    }

    if( oldpos != GetScreen()->m_Curseur )
    {
        curpos = GetScreen()->m_Curseur;
        GetScreen()->m_Curseur = oldpos;
        DrawPanel->CursorOff( DC );

        GetScreen()->m_Curseur = curpos;
        DrawPanel->CursorOn( DC );

        if( DrawPanel->ManageCurseur )
        {
            DrawPanel->ManageCurseur( DrawPanel, DC, TRUE );
        }
    }

    if( hotkey )
    {
        OnHotKey( DC, hotkey, NULL );
    }

    if( GetScreen()->IsRefreshReq() )
    {
        RedrawActiveWindow( DC, TRUE );
    }

    SetToolbars();
    Affiche_Status_Box();    /* Affichage des coord curseur */
}
