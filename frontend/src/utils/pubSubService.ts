type TopicNames =
  'requestFailed' |
  'requestSuccess' |
  'notLoginOrTokenExpired' |
  'loginSuccess'

class PubSubService {
  static instance: PubSubService;
  constructor() {
    console.log('PubSubService created')
    if (!PubSubService.instance) {
      PubSubService.instance = this;
    }
    return PubSubService.instance;
  }
  private topics: Record<TopicNames, Set<Function>> = {
    requestFailed: new Set(),
    requestSuccess: new Set(),
    notLoginOrTokenExpired: new Set(),
    loginSuccess: new Set()
  }

  subscribe(topicName: TopicNames, handler: Function) {
    this.topics[topicName].add(handler)
  }

  publish(topicName: TopicNames, ...args: any[]) {
    this.topics[topicName].forEach(handler => handler(...args))
  }
}

export default new PubSubService()