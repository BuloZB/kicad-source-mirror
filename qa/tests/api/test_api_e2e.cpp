/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright The KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>
#include <wx/filename.h>

#include "api_e2e_utils.h"


BOOST_AUTO_TEST_SUITE( ApiE2E )


BOOST_FIXTURE_TEST_CASE( ServerStartsAndResponds, API_SERVER_E2E_FIXTURE )
{
    BOOST_REQUIRE_MESSAGE( Start(), LastError() );

    BOOST_CHECK_MESSAGE( Client().GetVersion(), "GetVersion failed: " + Client().LastError() );
}


BOOST_FIXTURE_TEST_CASE( OpenSingleBoard, API_SERVER_E2E_FIXTURE )
{
    BOOST_REQUIRE_MESSAGE( Start(), LastError() );

    kiapi::common::types::DocumentSpecifier document;
    wxString                                testDataDir = wxString::FromUTF8( KI_TEST::GetPcbnewTestDataDir() );
    wxFileName                              boardPath( testDataDir, wxS( "api_kitchen_sink.kicad_pcb" ) );

    BOOST_REQUIRE_MESSAGE( Client().OpenDocument( boardPath.GetFullPath(), &document ),
                           "OpenDocument failed: " + Client().LastError() );

    BOOST_REQUIRE( document.type() == kiapi::common::types::DOCTYPE_PCB );
    BOOST_REQUIRE( boardPath.GetFullName().Matches( document.board_filename() ) );

    int footprintCount = 0;

    BOOST_REQUIRE_MESSAGE( Client().GetItemsCount( document, kiapi::common::types::KOT_PCB_FOOTPRINT, &footprintCount ),
                           "GetItems failed: " + Client().LastError() );

    BOOST_CHECK_GT( footprintCount, 0 );
}


BOOST_FIXTURE_TEST_CASE( SwitchBoards, API_SERVER_E2E_FIXTURE )
{
    BOOST_REQUIRE_MESSAGE( Start(), LastError() );

    wxString   testDataDir = wxString::FromUTF8( KI_TEST::GetPcbnewTestDataDir() );
    wxFileName boardPathA( testDataDir, wxS( "api_kitchen_sink.kicad_pcb" ) );
    wxFileName boardPathB( testDataDir, wxS( "padstacks.kicad_pcb" ) );

    kiapi::common::types::DocumentSpecifier documentA;

    BOOST_REQUIRE_MESSAGE( Client().OpenDocument( boardPathA.GetFullPath(), &documentA ),
                           "OpenDocument for first board failed: " + Client().LastError() );

    FOOTPRINT footprintA( nullptr );

    BOOST_REQUIRE_MESSAGE( Client().GetFirstFootprint( documentA, &footprintA ),
                           "GetFirstFootprint for first board failed: " + Client().LastError() );

    kiapi::common::types::DocumentSpecifier documentB;

    BOOST_REQUIRE_MESSAGE( Client().OpenDocument( boardPathB.GetFullPath(), &documentB ),
                           "OpenDocument for second board failed: " + Client().LastError() );

    FOOTPRINT footprintB( nullptr );

    BOOST_REQUIRE_MESSAGE( Client().GetFirstFootprint( documentB, &footprintB ),
                           "GetFirstFootprint for second board failed: " + Client().LastError() );

    BOOST_CHECK_NE( footprintA.Similarity( footprintB ), 1.0 );

    kiapi::common::ApiStatusCode closeStatus = kiapi::common::AS_UNKNOWN;

    BOOST_REQUIRE_MESSAGE( Client().CloseDocument( &documentB, &closeStatus ),
                           "CloseDocument failed: " + Client().LastError() );
    BOOST_CHECK_EQUAL( closeStatus, kiapi::common::AS_OK );

    BOOST_REQUIRE_MESSAGE( Client().CloseDocument( nullptr, &closeStatus ),
                           "CloseDocument after already closed failed: " + Client().LastError() );
    BOOST_CHECK_EQUAL( closeStatus, kiapi::common::AS_BAD_REQUEST );
}


BOOST_AUTO_TEST_SUITE_END()
