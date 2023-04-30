import 'dart:async';
import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'package:flutter/material.dart';
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(options: firebaseOptions);
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final FirebaseDatabase _database = FirebaseDatabase.instance;
  late DatabaseReference _alertsRef;
  late Stream<DataSnapshot> _alertsStream;
  final FlutterLocalNotificationsPlugin _localNotificationsPlugin =
      FlutterLocalNotificationsPlugin();

  @override
  void initState() {
  super.initState();
   _alertsRef = FirebaseDatabase.instance.reference().child('alerts');
   _alertsStream = _alertsRef.onValue;
}

  Future<void> _initializeLocalNotifications() async {
    const initializationSettingsAndroid =
        AndroidInitializationSettings('@mipmap/ic_launcher');
    final initializationSettings =
        InitializationSettings(android: initializationSettingsAndroid);
    await _localNotificationsPlugin.initialize(initializationSettings);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: const Text('Alerts')),
        body: StreamBuilder<DataSnapshot>(
          stream: _alertsStream,
          builder: (BuildContext context, AsyncSnapshot<DatabaseEvent> snapshot) {
            if (snapshot.hasData && snapshot.data.snapshot.exists) {
              print("Snapshot data: ${snapshot.data.snapshot.value}");
              return _buildAlertsList(snapshot.data.snapshot);
              } 
              else {
               return const Center(child: CircularProgressIndicator());
              }
           },
          },
        ),
      ),
    );
  }

  Widget _buildAlertsList(Map<dynamic, dynamic> alertsData) {
    final List<dynamic> alerts = alertsData.values.toList();
    return ListView.builder(
      itemCount: alerts.length,
      itemBuilder: (BuildContext context, int index) {
        final alert = alerts[index];
        return ListTile(
          title: Text(alert['title']),
          subtitle: Text(alert['message']),
          onTap: () => _showNotification(alert['title'], alert['message']),
        );
      },
    );
  }

  Future<void> _showNotification(String title, String message) async {
    const androidPlatformChannelSpecifics = AndroidNotificationDetails(
      'sentinelai_alerts_channel',
      'SentinelAI Alerts - Receive alerts from SentinelAI',
      importance: Importance.max,
      priority: Priority.high,
      showWhen: true,
    );
    const platformChannelSpecifics =
        NotificationDetails(android: androidPlatformChannelSpecifics);
    await _localNotificationsPlugin.show(0, title, message, platformChannelSpecifics);
  }
}
