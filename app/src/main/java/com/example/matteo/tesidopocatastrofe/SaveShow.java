package com.example.matteo.tesidopocatastrofe;

import android.app.Activity;
import android.content.Intent;
import android.media.Image;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

public class SaveShow extends AppCompatActivity {
ImageButton menuBottone;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_save_show);

        menuBottone = (ImageButton) findViewById(R.id.menuBottone);

        menuBottone.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent openMenu = new Intent(SaveShow.this, Menu.class);
                startActivity(openMenu);
            }
        });

    }
}
