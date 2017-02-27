package com.example.matteo.tesidopocatastrofe;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class Stack extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_stack);



        ImageButton menuBottone = (ImageButton)findViewById(R.id.btnStackMenu);
        menuBottone.setOnClickListener(new View.OnClickListener(){
                   @Override
                   public void onClick(View arg0){
                       //definisco l'intenzione
                       Intent openMenu = new Intent(Stack.this, Menu.class);
                       //richiamo l'activity
                       startActivity(openMenu);
                   }
               }
        );





    }
}
