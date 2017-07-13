#include <cstdio>
#include "json.hpp"

int main(int argc, char const *argv[])
{
	char * buffer = NULL;
    int length = 0;

    {
        ksJson * rootNode = ksJson_SetObject( ksJson_Create() );
        ksJson * vertices = ksJson_SetArray( ksJson_AddObjectMember( rootNode, "vertices" ) );
        for ( int i = 0; i < 3; i++ )
        {
            ksJson * vertex = ksJson_SetObject( ksJson_AddArrayElement( vertices ) );

            ksJson * position = ksJson_SetObject( ksJson_AddObjectMember( vertex, "position" ) );
            ksJson_SetFloat( ksJson_AddObjectMember( position, "x" ), (float)( ( i & 1 ) >> 0 ) );
            ksJson_SetFloat( ksJson_AddObjectMember( position, "y" ), (float)( ( i & 2 ) >> 1 ) );
            ksJson_SetFloat( ksJson_AddObjectMember( position, "z" ), (float)( ( i & 4 ) >> 2 ) );

            ksJson * normal = ksJson_SetObject( ksJson_AddObjectMember( vertex, "normal" ) );
            ksJson_SetFloat( ksJson_AddObjectMember( normal, "x" ), 0.5f );
            ksJson_SetFloat( ksJson_AddObjectMember( normal, "y" ), 0.6f );
            ksJson_SetFloat( ksJson_AddObjectMember( normal, "z" ), 0.7f );
        }
        ksJson * indices = ksJson_SetArray( ksJson_AddObjectMember( rootNode, "indices" ) );
        for ( unsigned int i = 0; i < 3; i++ )
        {
            ksJson_SetUint32( ksJson_AddArrayElement( indices ), i );
        }
        ksJson_WriteToBuffer( rootNode, &buffer, &length );
        ksJson_Destroy( rootNode );
    }

    {
        ksJson * rootNode = ksJson_Create();
        if ( ksJson_ReadFromBuffer( rootNode, buffer, NULL ) )
        {
            const ksJson * vertices = ksJson_GetMemberByName( rootNode, "vertices" );
            for ( int i = 0; i < ksJson_GetMemberCount( vertices ); i++ )
            {
                const ksJson * vertex = ksJson_GetMemberByIndex( vertices, i );

                const ksJson * position = ksJson_GetMemberByName( vertex, "position" );
                const float position_x = ksJson_GetFloat( ksJson_GetMemberByName( position, "x" ), 0.0f );
                const float position_y = ksJson_GetFloat( ksJson_GetMemberByName( position, "y" ), 0.0f );
                const float position_z = ksJson_GetFloat( ksJson_GetMemberByName( position, "z" ), 0.0f );

                const ksJson * normal = ksJson_GetMemberByName( vertex, "normal" );
                const float normal_x = ksJson_GetFloat( ksJson_GetMemberByName( normal, "x" ), 0.0f );
                const float normal_y = ksJson_GetFloat( ksJson_GetMemberByName( normal, "y" ), 0.0f );
                const float normal_z = ksJson_GetFloat( ksJson_GetMemberByName( normal, "z" ), 0.0f );
            }
            const ksJson * indices = ksJson_GetMemberByName( rootNode, "indices" );
            for ( int i = 0; i < ksJson_GetMemberCount( indices ); i++ )
            {
                const unsigned int index = ksJson_GetUint32( ksJson_GetMemberByIndex( indices, i ), 0 );
            }
        }
        ksJson_Destroy( rootNode );
    }
    printf("%s\n", buffer);
    free( buffer );
	return 0;
}