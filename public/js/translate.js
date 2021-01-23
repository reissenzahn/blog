

function playTranslation(text) {
  fetch('https://texttospeech.googleapis.com/v1beta1/text:synthesize', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'X-Goog-Api-Key': 'AIzaSyD_THt2eiy0ehZuU5pzHUnAsV1_inHeS4k',
    },
    body: JSON.stringify({
      input: {
        text: text,
      },
      voice: {
        languageCode: 'zh-CN',
        ssmlGender: 'FEMALE',
      },
      audioConfig: {
        audioEncoding: 'MP3',
      },
    }),
  })
  .then((response) => response.json())
  .then((json) => {
    new Audio('data:audio/wav;base64,' + json['audioContent']).play();
  });
}
