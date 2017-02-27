package com.example.matteo.tesidopocatastrofe;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.IOException;
import java.util.UUID;


public class SingleChannel extends AppCompatActivity implements SeekBar.OnSeekBarChangeListener {

    static final int MinValue = 1;
    static final int MaxValue = 255;
    int simboloInizio = 500, simboloFine = 501;

    int i;
    int numCanali = 6;
    int arrayValoriFaders[] = new int[numCanali]; //<-- stato della interfaccia*/
    //String stringaValori;
    //    {21,32,43,54}

    //Attributi Bluetooth
    String address = null;
    private ProgressDialog progress;

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;


    private static boolean isBtConnected = false;
    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // dichiaro foglie
    Button btnSingleClear, btnSingleBO, btnSingleSave, btnSingleHL, btnSingleNext, btnSinglePrev;
    //ImageButton menuBottone;
    SeekBar fader[] = new SeekBar[numCanali];
    TextView numFader[] = new TextView[numCanali];
    TextView valFader[] = new TextView[numCanali];
    int numFaderViewId[];
    int faderViewId[];
    int valFaderViewId[];


    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent newint = getIntent();
        //address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS); //receive the address of the bluetooth device
        address = DeviceList.getBluetoothDeviceID();

        setContentView(R.layout.activity_single_channel);

        if (btSocket == null) { //If the btSocket is busy
            new ConnectBT().execute(); //Call the class to connect
        }



        // inizializzazione foglie
        //menuBottone = (ImageButton) findViewById(R.id.btnSingleMenu);
        btnSingleClear = (Button) findViewById(R.id.btnSingleClear);
        btnSingleBO = (Button) findViewById(R.id.btnSingleBO);
        btnSingleSave = (Button) findViewById(R.id.btnSingleSave);
        btnSingleHL = (Button) findViewById(R.id.btnSingleHL);
        btnSingleNext = (Button) findViewById(R.id.btnSingleNext);
        btnSinglePrev = (Button) findViewById(R.id.btnSinglePrev);


        valFaderViewId[0]=R.id.fader1;
        valFaderViewId[1]=R.id.fader2;
        valFaderViewId[2]=R.id.fader3;
        valFaderViewId[3]=R.id.fader4;
        valFaderViewId[4]=R.id.fader5;
        valFaderViewId[5]=R.id.fader6;

        for(i=0; i< valFader.length; i++) fader[i] = (SeekBar) findViewById(valFaderViewId[i]);

        for (i = 0; i < numCanali; i++) arrayValoriFaders[i] = 0;

        Log.e("TAG--", "tester");

        String buf = "numFader";

        for (i = 0; i < numCanali; i++) {
            int textId = getResources().getIdentifier("numFader" + i, "id", getPackageName());
            numFader[i] = (TextView) findViewById(textId);
            Log.e("TAG--", "buf");
        }


        valFader[0] = (TextView) findViewById(R.id.valFader1);
        valFader[1] = (TextView) findViewById(R.id.valFader2);
        valFader[2] = (TextView) findViewById(R.id.valFader3);
        valFader[3] = (TextView) findViewById(R.id.valFader4);
        valFader[4] = (TextView) findViewById(R.id.valFader5);
        valFader[5] = (TextView) findViewById(R.id.valFader6);

        String txtValore1 = Integer.toString(arrayValoriFaders[0]);
        String txtValore2 = Integer.toString(arrayValoriFaders[1]);
        String txtValore3 = Integer.toString(arrayValoriFaders[2]);
        String txtValore4 = Integer.toString(arrayValoriFaders[3]);
        String txtValore5 = Integer.toString(arrayValoriFaders[4]);
        String txtValore6 = Integer.toString(arrayValoriFaders[5]);

        valFader[0].setText(txtValore1);
        valFader[1].setText(txtValore2);
        valFader[2].setText(txtValore3);
        valFader[3].setText(txtValore4);
        valFader[4].setText(txtValore5);
        valFader[5].setText(txtValore6);


        for (SeekBar element : fader) element.setOnSeekBarChangeListener(this);


        btnSingleClear.setOnClickListener(new View.OnClickListener() {
                                              @Override
                                              public void onClick(View view) {
                                                  for (i = 0; i < numCanali; i++) arrayValoriFaders[i] = 0;
                                                  for (TextView element : valFader) element.setText(0 + "%");
                                                  for (SeekBar element : fader) element.setProgress(0);
                                                  //inviaArray(arrayValoriFaders);
                                                  inviaTuttiIValori(arrayValoriFaders);
                                              }
                                          }
        );


        btnSingleHL.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                CharSequence text = "Hi-Light in sviluppo";
                int duration = Toast.LENGTH_SHORT;

                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
        btnSingleNext.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                CharSequence text = "Hi-Light in sviluppo";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
        btnSinglePrev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                CharSequence text = "Hi-Light in sviluppo";
                int duration = Toast.LENGTH_SHORT;

                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
        btnSingleSave.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                CharSequence text = "Gestione Files in sviluppo";
                int duration = Toast.LENGTH_SHORT;

                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
        btnSingleBO.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Context context = getApplicationContext();
                CharSequence text = "Mi scollego dal device bluetooth ";
                if (btSocket != null) { //If the btSocket is busy
                    try {
                        btSocket.close(); //close connection
                    } catch (IOException e) {
                        msg("Error");
                    }
                }
                finish(); //return to the first layout
                int duration = Toast.LENGTH_SHORT;

                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        });
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle presses on the action bar items
        switch (item.getItemId()) {
            case R.id.btnActionMenu:
                Intent openMenu = new Intent(SingleChannel.this, com.example.matteo.tesidopocatastrofe.Menu.class);
                startActivity(openMenu);
                //apriMenu();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }



    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        Log.v("", "" + seekBar);
        if (fromUser) {
            /*
            arrayValoriFaders[numero] = progress;
            valFader[numero].setText(String.valueOf(progress + "%"));
            inviaArray(arrayValoriFaders);
            /**/
            switch (seekBar.getId()) {
                case R.id.fader1:
                    arrayValoriFaders[0] = progress;
//                     valFader[0].setText(String.valueOf(seekBar.getId()));
                   valFader[0].setText(String.valueOf(progress));
//                    inviaArray(arrayValoriFaders);
                    inviaSingoloValore(0, arrayValoriFaders);
                    break;

                case R.id.fader2:
                    arrayValoriFaders[1] = progress;
//                    valFader[1].setText(String.valueOf(seekBar.getId()));
                    valFader[1].setText(String.valueOf(progress));
//                    inviaArray(arrayValoriFaders);
                    inviaSingoloValore(1, arrayValoriFaders);
                    break;

                case R.id.fader3:
                    arrayValoriFaders[2] = progress;
//                    valFader[2].setText(String.valueOf(seekBar.getId()));
                    valFader[2].setText(String.valueOf(progress));
 //                   inviaArray(arrayValoriFaders);
                    inviaSingoloValore(2, arrayValoriFaders);
                    break;

                case R.id.fader4:
                    arrayValoriFaders[3] = progress;
//                    valFader[3].setText(String.valueOf(seekBar.getId()));
                    valFader[3].setText(String.valueOf(progress));
//                    inviaArray(arrayValoriFaders);
                    inviaSingoloValore(3, arrayValoriFaders);
                    break;

                case R.id.fader5:
                    arrayValoriFaders[4] = progress;
//                    valFader[4].setText(String.valueOf(seekBar.getId()));
                    valFader[4].setText(String.valueOf(progress));
//                    inviaArray(arrayValoriFaders);
                    inviaSingoloValore(4, arrayValoriFaders);
                    break;

                case R.id.fader6:
                    arrayValoriFaders[5] = progress;
//                    valFader[5].setText(String.valueOf(seekBar.getId()));
                    valFader[5].setText(String.valueOf(progress));
//                    inviaArray(arrayValoriFaders);
                    inviaSingoloValore(5, arrayValoriFaders);
                    break;


            }
        }
    }


    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    GRANDI ISSUES DI OGGI   /////////////////////////////////
// devo riuscire a comunicare un array di valori via bluetooth all'arduino e gestirli
// si crea una gran confuzione nella comunicazione, forse per colpa dei tempi.
// Buonanotte.
/////////////////////////////////////////////////////////////////////////////////////////////

    public void inviaSingoloValore(int i ,int[] arrayValoriFaders){
        //per gestire gli errori provocati dalla comunicazione contemporanea dell'arduino
        // sia col bluetooth che con l'UART per la gestione dell'uscita RS485,
        // ho valutato dapprima l'invio dei singoli con pacchetti dedicati.

        /* !n-v?
        * ! -> primo carattere
        * n -> numero canale
        * - -> divisore
        * v -> valore DMX 0-255
        * ? -> ultimo carattere
        * minimo 5 caratteri, massimo 9 caratteri
        * */

        String valore;
        valore = "!"+i+"-"+String.valueOf(arrayValoriFaders[i])+"&";
        try {
            btSocket.getOutputStream().write(valore.getBytes());
            aspetta(1); // delay in ms
        } catch (IOException e) {
        }
    }

    public void inviaTuttiIValori(int[] arrayValoriFaders){
        String valore;
        for(i=0;i<numCanali;i++){
            valore = "!"+i+"-"+String.valueOf(arrayValoriFaders[i])+"&";
            try {
                btSocket.getOutputStream().write(valore.getBytes());
                aspetta(1); // delay in ms
            } catch (IOException e) {
            }
        }
    }

    // Per diminuire però il carico computazionale e i tempi di invio, ho poi valutato l'utilizzo di una checksum:
    // inviare pacchetti contenenti gruppi di 6/12 valori seguiti dalla somma dei valori.
    // Il microcontrollore, al momento della ricezione, prima di memorizzare i dati ricevuti,
    // ne calcola la somma e la confronta con quella ricevuta.


/*
public void inviaArray(int[] arrayValoriFaders) {
    try {
        int somma = 0;

        //INVIO SERIALE DI TUTTI I VALORI DELL'ARRAY

            String stringaValori="!";
            for(i=0;i<numCanali;i++){
                stringaValori+=String.valueOf(arrayValoriFaders[i]+".");
                somma +=arrayValoriFaders[i];
            }
            stringaValori+=String.valueOf(somma);
            stringaValori+="&";

*/

           /*String stringaValori= "!"
                   +String.valueOf(arrayValoriFaders[0])+"."
                   +String.valueOf(arrayValoriFaders[1])+"."
                   +String.valueOf(arrayValoriFaders[2])+"."
                   +String.valueOf(arrayValoriFaders[3])+"."
                   +String.valueOf(arrayValoriFaders[4])+"."
                   +String.valueOf(arrayValoriFaders[5])+"&";
*/
/*
            btSocket.getOutputStream().write(stringaValori.getBytes());
            aspetta(20); // delay in ms
        } catch (IOException e) {
        }

}
*/



    public void aspetta(int millisecondi) {
        try {
            Thread.sleep(millisecondi);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    ;

    // fast way to call Toast
    private void msg(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }




    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("SingleChannel Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> { // UI thread
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute() {
            if (DeviceList.getBluetoothDeviceID() == ""){
                if (btSocket != null) { //If the btSocket is busy
                    try {
                        btSocket.close(); //close connection
                    } catch (IOException e) {
                        msg("Error");
                    }
                }
            }
            progress = ProgressDialog.show(SingleChannel.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }


        @Override
        protected Void doInBackground(Void... devices) { //while the progress dialog is shown, the connection is done in background
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) { //after the doInBackground, it checks if everything went fine
            super.onPostExecute(result);

            if (!ConnectSuccess) {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            } else {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }
}





