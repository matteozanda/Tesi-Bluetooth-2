package com.example.matteo.tesidopocatastrofe;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class Scenes extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scenes);



        ImageButton menuBottone = (ImageButton)findViewById(R.id.btnScenesMenu);
        menuBottone.setOnClickListener(new View.OnClickListener(){
                   @Override
                   public void onClick(View arg0){
                       //definisco l'intenzione
                       Intent openMenu = new Intent(Scenes.this, Menu.class);
                       //richiamo l'activity
                       startActivity(openMenu);
                   }
               }
        );





    }
}
