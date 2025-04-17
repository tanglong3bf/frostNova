declare interface BaseResp {
  code?: number
  data?: any
}

declare interface SuccessResp extends BaseResp {
  message: string
  error?: never
}

declare interface ErrorResp extends BaseResp {
  error: string
  message?: never
}
declare type Resp = SuccessResp | ErrorResp

declare type NullableResp = Resp | null